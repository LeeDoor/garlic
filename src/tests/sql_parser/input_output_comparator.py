import sys
import subprocess
import re
import time
import select
import os
from pathlib import Path
from typing import List, Tuple, Optional

ERROR_WILDCARDS = {
    "<ERROR>": lambda line: line.startswith("[") and "_ERROR]" in line,
    "<LEXICAL_ERROR>": lambda line: line.startswith("[LEXICAL_ERROR]"),
    "<SYNTAX_ERROR>": lambda line: line.startswith("[SYNTAX_ERROR]"),
    "<SEMANTIC_ERROR>": lambda line: line.startswith("[SEMANTIC_ERROR]"),
}

ANSI_RESET = "\033[0m"
ANSI_BOLD = "\033[1m"
ANSI_RED = "\033[31m"
ANSI_GREEN = "\033[32m"
ANSI_YELLOW = "\033[33m"

def test_file_sort_key(path: Path) -> Tuple[int, str]:
    name = path.stem
    match = re.match(r"^(\d+)", name)
    if match:
        return (int(match.group(1)), name)
    return (sys.maxsize, name)

def find_regular_test_pairs() -> List[Tuple[Path, Path]]:
    script_dir = Path(__file__).parent
    i_files = sorted(script_dir.glob("*.dat"), key=test_file_sort_key)
    pairs = []

    for i_file in i_files:
        e_file = i_file.with_suffix('.ans')
        if e_file.exists():
            pairs.append((i_file, e_file))

    return pairs

def find_error_tests(directory_name: str) -> List[Path]:
    script_dir = Path(__file__).parent / directory_name
    if not script_dir.exists():
        return []
    return sorted(script_dir.glob("*.dat"), key=test_file_sort_key)

def read_file_content(file_path: Path, max_lines: Optional[int] = None) -> str:
    with open(file_path, 'r') as f:
        if max_lines:
            lines = []
            for i, line in enumerate(f, 1): 
                lines.append(f"{i:3d}: {line.rstrip('\n')}")
            return '\n'.join(lines)
        else:
            return f.read()

def read_file_content_with_line_numbers(file_path: Path) -> str:
    """Read file content and add line numbers to each line."""
    with open(file_path, 'r') as f:
        lines = []
        for i, line in enumerate(f, 1): 
            lines.append(f"{i:3d}: {line.rstrip('\n')}")
        return '\n'.join(lines)

def estimate_output_limit(expected_output: Optional[str]) -> int:
    if expected_output is None:
        return 64 * 1024

    lines = normalize_lines(expected_output)
    expected_chars = sum(len(line) + 1 for line in lines)
    expected_error_lines = sum(1 for line in lines if line in ERROR_WILDCARDS)

    # Error lines contain location/context; reserve extra space.
    calculated = expected_chars + expected_error_lines * 200 + 1024
    return max(4096, min(calculated, 256 * 1024))

def run_executable(executable: str, input_content: str, expected_output: Optional[str] = None) -> subprocess.CompletedProcess:
    max_output_bytes = estimate_output_limit(expected_output)
    max_runtime_seconds = 1.0

    try:
        process = subprocess.Popen(
            [executable],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=False,
        )

        if process.stdin is not None:
            process.stdin.write(input_content.encode())
            process.stdin.close()

        output = bytearray()
        timed_out = False
        output_limited = False
        start_time = time.monotonic()

        if process.stdout is None:
            raise RuntimeError("Failed to capture process stdout")

        out_fd = process.stdout.fileno()
        while True:
            elapsed = time.monotonic() - start_time
            if elapsed > max_runtime_seconds:
                timed_out = True
                process.terminate()
                break

            if len(output) >= max_output_bytes:
                output_limited = True
                process.terminate()
                break

            if process.poll() is not None:
                break

            remaining = max_output_bytes - len(output)
            wait_timeout = min(0.05, max(0.0, max_runtime_seconds - elapsed))
            readable, _, _ = select.select([out_fd], [], [], wait_timeout)
            if not readable:
                continue

            chunk = os.read(out_fd, min(4096, remaining))
            if not chunk:
                break
            output.extend(chunk)

        # Drain remaining output quickly after process end/termination.
        drain_deadline = time.monotonic() + 0.2
        while len(output) < max_output_bytes and time.monotonic() < drain_deadline:
            readable, _, _ = select.select([out_fd], [], [], 0.01)
            if not readable:
                break
            chunk = os.read(out_fd, min(4096, max_output_bytes - len(output)))
            if not chunk:
                break
            output.extend(chunk)

        try:
            process.wait(timeout=0.2)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait(timeout=0.2)

        class ExecResult:
            def __init__(self, returncode: int, stdout: str, timed_out: bool, output_limited: bool):
                self.returncode = returncode
                self.stdout = stdout
                self.timed_out = timed_out
                self.output_limited = output_limited

        return ExecResult(
            process.returncode,
            output.decode(errors="replace"),
            timed_out,
            output_limited,
        )
    except FileNotFoundError:
        class MissingExecutable:
            returncode = 127
            stdout = f"Error: Executable '{executable}' not found"
            timed_out = False
            output_limited = False
        return MissingExecutable()

def normalize_lines(content: str) -> List[str]:
    return content.splitlines()

def line_matches(expected_line: str, actual_line: str) -> bool:
    checker = ERROR_WILDCARDS.get(expected_line)
    if checker is not None:
        return checker(actual_line)
    return expected_line == actual_line

def outputs_match(expected: str, actual: str) -> bool:
    expected_lines = normalize_lines(expected)
    actual_lines = normalize_lines(actual)
    if len(expected_lines) != len(actual_lines):
        return False
    return all(
        line_matches(exp, act)
        for exp, act in zip(expected_lines, actual_lines)
    )

def style(text: str, *codes: str) -> str:
    return "".join(codes) + text + ANSI_RESET

def truncate_for_width(text: str, width: int) -> str:
    if len(text) <= width:
        return text
    if width <= 3:
        return text[:width]
    return text[:width - 3] + "..."

def get_diff(actual: str, expected: str, width: int = 180) -> str:
    actual_lines = actual.splitlines()
    expected_lines = expected.splitlines()
    
    # Make both lists the same length by padding with empty strings
    max_len = max(len(actual_lines), len(expected_lines))
    actual_lines.extend([''] * (max_len - len(actual_lines)))
    expected_lines.extend([''] * (max_len - len(expected_lines)))
    
    marker_width = 8
    content_width = max(40, width - marker_width)
    left_width = content_width // 2
    right_width = content_width - left_width
    
    result = []
    result.append(" " * left_width + " " * marker_width + " " * right_width)
    result.append(style("ACTUAL", ANSI_BOLD).ljust(left_width) + " " * marker_width + style("EXPECTED", ANSI_BOLD).ljust(right_width))
    result.append("-" * left_width + "-" * marker_width + "-" * right_width)
    
    for actual_line, expected_line in zip(actual_lines, expected_lines):
        expected_is_wildcard = expected_line in ERROR_WILDCARDS
        if line_matches(expected_line, actual_line):
            if expected_is_wildcard:
                marker = style("~OK~", ANSI_BOLD, ANSI_GREEN)
            else:
                marker = style(" OK ", ANSI_GREEN)
        elif actual_line == "":
            marker = style(" << ", ANSI_BOLD, ANSI_YELLOW)
        elif expected_line == "":
            marker = style(" >> ", ANSI_BOLD, ANSI_YELLOW)
        else:
            marker = style(" !! ", ANSI_BOLD, ANSI_RED)
        
        actual_disp = truncate_for_width(actual_line, left_width)
        expected_disp = truncate_for_width(expected_line, right_width)
        
        # Format the line
        line = f"{actual_disp:<{left_width}} {marker} {expected_disp:<{right_width}}"
        result.append(line)
    
    return '\n'.join(result)

def run_test(executable: str, input_file: Path, expected_file: Path, max_input_lines: int = 10) -> bool:
    test_name = input_file.stem
    input_content_full = read_file_content(input_file)
    input_content_preview = read_file_content(input_file, max_input_lines)
    expected_output_with_numbers = read_file_content_with_line_numbers(expected_file)
    expected_output = read_file_content(expected_file)
    process = run_executable(executable, input_content_full, expected_output=expected_output)
    actual_output = process.stdout
    if outputs_match(expected_output, actual_output):
        return True

    print(f"test name: {test_name}")
    print("===INPUT===")
    print(input_content_preview)
    print("===OUTPUTS===")
    actual_lines = actual_output.splitlines()
    numbered_actual = '\n'.join([f"{i+1:3d}: {line}" for i, line in enumerate(actual_lines)])
    print(numbered_actual)
    print(f"===RETURN CODE===\n{process.returncode}")
    if getattr(process, "timed_out", False):
        print("===EXECUTION LIMIT===\nProcess timed out")
    if getattr(process, "output_limited", False):
        print("===EXECUTION LIMIT===\nOutput limit reached")
    print("===EXPECTED (with line numbers)===")
    print(expected_output_with_numbers)
    print("===DIFFERENCE===")
    print(get_diff(actual_output, expected_output))

    return False

def run_error_prefix_test(executable: str, input_file: Path, required_prefix: str, max_input_lines: int = 10) -> bool:
    test_name = input_file.stem
    input_content_full = read_file_content(input_file)
    input_content_preview = read_file_content(input_file, max_input_lines)
    expected_output = required_prefix + "\n"
    process = run_executable(executable, input_content_full, expected_output=expected_output)

    output = process.stdout.lstrip()
    if output.startswith(required_prefix):
        return True

    print(f"test name: {test_name}")
    print("===INPUT===")
    print(input_content_preview)
    print(f"===EXPECTED PREFIX===\n{required_prefix}")
    print("===OUTPUT===")
    print(process.stdout)
    print(f"===RETURN CODE===\n{process.returncode}")
    if getattr(process, "timed_out", False):
        print("===EXECUTION LIMIT===\nProcess timed out")
    if getattr(process, "output_limited", False):
        print("===EXECUTION LIMIT===\nOutput limit reached")
    return False

def main():
    if len(sys.argv) != 2:
        print("Usage: python test_runner.py <executable>")
        sys.exit(1)

    executable = sys.argv[1]

    test_pairs = find_regular_test_pairs()
    lexical_error_tests = find_error_tests("LEXICAL_ERROR")
    syntax_error_tests = find_error_tests("SYNTAX_ERROR")
    semantic_error_tests = find_error_tests("SEMANTIC_ERROR")

    if not test_pairs and not lexical_error_tests and not syntax_error_tests and not semantic_error_tests:
        script_dir = Path(__file__).parent
        print(f"No test files found in {script_dir}")
        sys.exit(0)

    print(
        f"Found {len(test_pairs)} regular test(s), "
        f"{len(lexical_error_tests)} lexical error test(s), "
        f"{len(syntax_error_tests)} syntax error test(s), "
        f"{len(semantic_error_tests)} semantic error test(s)"
    )

    for i, (input_file, expected_file) in enumerate(test_pairs, 1):
        print(f"Running regular test {i}: {input_file.name}")
        if not run_test(executable, input_file, expected_file):
            print(f"\nTest failed: {input_file.stem}")
            sys.exit(1)

    for i, input_file in enumerate(lexical_error_tests, 1):
        print(f"Running lexical error test {i}: {input_file.name}")
        if not run_error_prefix_test(executable, input_file, "[LEXICAL_ERROR]"):
            print(f"\nTest failed: {input_file.stem}")
            sys.exit(1)

    for i, input_file in enumerate(syntax_error_tests, 1):
        print(f"Running syntax error test {i}: {input_file.name}")
        if not run_error_prefix_test(executable, input_file, "[SYNTAX_ERROR]"):
            print(f"\nTest failed: {input_file.stem}")
            sys.exit(1)

    for i, input_file in enumerate(semantic_error_tests, 1):
        print(f"Running semantic error test {i}: {input_file.name}")
        if not run_error_prefix_test(executable, input_file, "[SEMANTIC_ERROR]"):
            print(f"\nTest failed: {input_file.stem}")
            sys.exit(1)

    print("\nAll tests passed!")
    sys.exit(0)

if __name__ == "__main__":
    main()
