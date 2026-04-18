import os
import re
import select
import subprocess
import sys
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable

ERROR_WILDCARDS = {
    "<ERROR>": lambda line: line.startswith("[") and "_ERROR]" in line,
    "<LEXICAL_ERROR>": lambda line: line.startswith("[LEXICAL_ERROR]"),
    "<SYNTAX_ERROR>": lambda line: line.startswith("[SYNTAX_ERROR]"),
    "<SEMANTIC_ERROR>": lambda line: line.startswith("[SEMANTIC_ERROR]"),
    "<RUNTIME_ERROR>": lambda line: line.startswith("[RUNTIME_ERROR]"),
}
LOCATION_ERROR_PREFIX_RE = re.compile(r"^\[[A-Z_]+_ERROR\] at \[\d+\.\d+\]$")
SPECIAL_MODES = {
    "location_tests": ("location", ()),
    "location_tests_cli": ("location_cli", ("--use-cli-output",)),
}

ANSI_RESET = "\033[0m"
ANSI_BOLD = "\033[1m"
ANSI_RED = "\033[31m"
ANSI_GREEN = "\033[32m"
ANSI_YELLOW = "\033[33m"


@dataclass(frozen=True)
class ExecResult:
    returncode: int
    stdout: str
    timed_out: bool = False
    output_limited: bool = False


@dataclass(frozen=True)
class TestCase:
    group: str
    input_file: Path
    expected_file: Path
    mode: str = "default"
    extra_args: tuple[str, ...] = ()


def test_file_sort_key(path: Path) -> tuple[int, str]:
    match = re.match(r"^(\d+)", path.stem)
    if match:
        return int(match.group(1)), path.stem
    return sys.maxsize, path.stem


def iter_pairs(directory: Path) -> Iterable[tuple[Path, Path]]:
    if not directory.exists():
        return
    for input_file in sorted(directory.glob("*.dat"), key=test_file_sort_key):
        expected_file = input_file.with_suffix(".ans")
        if expected_file.exists():
            yield input_file, expected_file


def collect_cases(root: Path) -> list[TestCase]:
    cases: list[TestCase] = []

    for input_file, expected_file in iter_pairs(root):
        cases.append(TestCase("regular", input_file, expected_file))

    for child in sorted(root.iterdir(), key=lambda path: path.name):
        if not child.is_dir() or child.name.startswith("__"):
            continue
        mode, extra_args = SPECIAL_MODES.get(child.name, ("default", ()))
        for input_file, expected_file in iter_pairs(child):
            cases.append(TestCase(child.name, input_file, expected_file, mode, extra_args))

    return cases


def normalize_lines(content: str) -> list[str]:
    return content.splitlines()


def number_lines(text: str) -> str:
    return "\n".join(f"{index:3d}: {line}" for index, line in enumerate(normalize_lines(text), 1))


def estimate_output_limit(expected_output: str) -> int:
    lines = normalize_lines(expected_output)
    expected_chars = sum(len(line) + 1 for line in lines)
    error_lines = sum(
        1
        for line in lines
        if line in ERROR_WILDCARDS or LOCATION_ERROR_PREFIX_RE.match(line)
    )
    calculated = expected_chars + error_lines * 200 + 1024
    return max(4096, min(calculated, 256 * 1024))


def run_executable(executable: str, input_content: str, expected_output: str, extra_args: tuple[str, ...]) -> ExecResult:
    max_output_bytes = estimate_output_limit(expected_output)
    max_runtime_seconds = 1.0

    try:
        process = subprocess.Popen(
            [executable, *extra_args],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=False,
        )
    except FileNotFoundError:
        return ExecResult(127, f"Error: Executable '{executable}' not found")

    if process.stdin is not None:
        process.stdin.write(input_content.encode())
        process.stdin.close()

    if process.stdout is None:
        raise RuntimeError("Failed to capture process stdout")

    output = bytearray()
    timed_out = False
    output_limited = False
    start_time = time.monotonic()
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

        wait_timeout = min(0.05, max(0.0, max_runtime_seconds - elapsed))
        readable, _, _ = select.select([out_fd], [], [], wait_timeout)
        if not readable:
            continue

        remaining = max_output_bytes - len(output)
        chunk = os.read(out_fd, min(4096, remaining))
        if not chunk:
            break
        output.extend(chunk)

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

    return ExecResult(
        process.returncode,
        output.decode(errors="replace"),
        timed_out,
        output_limited,
    )


def strip_cli_prompt_prefix(line: str) -> str:
    while line and not line.startswith("[") and not line[0].isdigit():
        space_idx = line.find(" ")
        if space_idx < 0:
            break
        line = line[space_idx + 1:]
    return line


def prepare_actual_lines(mode: str, actual_output: str) -> list[str]:
    lines = normalize_lines(actual_output)
    if mode != "location_cli":
        return lines

    prepared: list[str] = []
    for line in lines:
        stripped = strip_cli_prompt_prefix(line)
        if stripped:
            prepared.append(stripped)
    return prepared


def line_matches(expected_line: str, actual_line: str, mode: str) -> bool:
    if mode in {"location", "location_cli"} and LOCATION_ERROR_PREFIX_RE.match(expected_line):
        return actual_line.startswith(expected_line)

    checker = ERROR_WILDCARDS.get(expected_line)
    if checker is not None:
        return checker(actual_line)

    return expected_line == actual_line


def outputs_match(expected_output: str, actual_output: str, mode: str) -> bool:
    expected_lines = normalize_lines(expected_output)
    actual_lines = prepare_actual_lines(mode, actual_output)
    if len(expected_lines) == 1 and expected_lines[0] in ERROR_WILDCARDS:
        return bool(actual_lines) and line_matches(expected_lines[0], actual_lines[0], mode)
    if len(expected_lines) != len(actual_lines):
        return False
    return all(line_matches(expected, actual, mode) for expected, actual in zip(expected_lines, actual_lines))


def style(text: str, *codes: str) -> str:
    return "".join(codes) + text + ANSI_RESET


def truncate_for_width(text: str, width: int) -> str:
    if len(text) <= width:
        return text
    if width <= 3:
        return text[:width]
    return text[: width - 3] + "..."


def get_diff(actual_output: str, expected_output: str, mode: str, width: int = 180) -> str:
    actual_lines = prepare_actual_lines(mode, actual_output)
    expected_lines = normalize_lines(expected_output)

    max_len = max(len(actual_lines), len(expected_lines))
    actual_lines.extend([""] * (max_len - len(actual_lines)))
    expected_lines.extend([""] * (max_len - len(expected_lines)))

    marker_width = 8
    content_width = max(40, width - marker_width)
    left_width = content_width // 2
    right_width = content_width - left_width

    result = [
        " " * left_width + " " * marker_width + " " * right_width,
        style("ACTUAL", ANSI_BOLD).ljust(left_width) + " " * marker_width + style("EXPECTED", ANSI_BOLD).ljust(right_width),
        "-" * left_width + "-" * marker_width + "-" * right_width,
    ]

    for actual_line, expected_line in zip(actual_lines, expected_lines):
        if line_matches(expected_line, actual_line, mode):
            marker = style("~OK~", ANSI_BOLD, ANSI_GREEN) if expected_line in ERROR_WILDCARDS else style(" OK ", ANSI_GREEN)
        elif actual_line == "":
            marker = style(" << ", ANSI_BOLD, ANSI_YELLOW)
        elif expected_line == "":
            marker = style(" >> ", ANSI_BOLD, ANSI_YELLOW)
        else:
            marker = style(" !! ", ANSI_BOLD, ANSI_RED)

        result.append(
            f"{truncate_for_width(actual_line, left_width):<{left_width}} "
            f"{marker} "
            f"{truncate_for_width(expected_line, right_width):<{right_width}}"
        )

    return "\n".join(result)


def report_failure(case: TestCase, expected_output: str, result: ExecResult) -> None:
    print(f"test name: {case.input_file.stem}")
    print("===INPUT===")
    print(number_lines(case.input_file.read_text()))
    print("===OUTPUTS===")
    print(number_lines(result.stdout))
    print(f"===RETURN CODE===\n{result.returncode}")
    if result.timed_out:
        print("===EXECUTION LIMIT===\nProcess timed out")
    if result.output_limited:
        print("===EXECUTION LIMIT===\nOutput limit reached")
    print("===EXPECTED (with line numbers)===")
    print(number_lines(expected_output))
    print("===DIFFERENCE===")
    print(get_diff(result.stdout, expected_output, case.mode))


def run_case(executable: str, case: TestCase) -> bool:
    input_content = case.input_file.read_text()
    expected_output = case.expected_file.read_text()
    result = run_executable(executable, input_content, expected_output, case.extra_args)
    if outputs_match(expected_output, result.stdout, case.mode):
        return True
    report_failure(case, expected_output, result)
    return False


def main() -> None:
    if len(sys.argv) != 2:
        print("Usage: python test_runner.py <executable>")
        sys.exit(1)

    executable = sys.argv[1]
    root = Path(__file__).parent
    cases = collect_cases(root)

    if not cases:
        print(f"No test files found in {root}")
        sys.exit(0)

    counts: dict[str, int] = {}
    for case in cases:
        counts[case.group] = counts.get(case.group, 0) + 1
    print("Found " + ", ".join(f"{count} {group} test(s)" for group, count in counts.items()))

    running: dict[str, int] = {}
    for case in cases:
        running[case.group] = running.get(case.group, 0) + 1
        print(f"Running {case.group} test {running[case.group]}: {case.input_file.name}")
        if not run_case(executable, case):
            print(f"\nTest failed: {case.input_file.stem}")
            sys.exit(1)

    print("\nAll tests passed!")


if __name__ == "__main__":
    main()
