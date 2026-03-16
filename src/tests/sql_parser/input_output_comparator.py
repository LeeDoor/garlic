import os
import sys
import subprocess
import difflib
import re
from pathlib import Path
from typing import List, Tuple, Optional

def test_file_sort_key(path: Path) -> Tuple[int, str]:
    name = path.stem
    match = re.match(r"^(\d+)", name)
    if match:
        return (int(match.group(1)), name)
    return (sys.maxsize, name)

def find_test_pairs() -> List[Tuple[Path, Path]]:
    script_dir = Path(__file__).parent
    i_files = sorted(script_dir.glob("*.dat"), key=test_file_sort_key)
    pairs = []

    for i_file in i_files:
        e_file = i_file.with_suffix('.ans')
        if e_file.exists():
            pairs.append((i_file, e_file))

    return pairs

def read_file_content(file_path: Path, max_lines: Optional[int] = None) -> str:
    with open(file_path, 'r') as f:
        if max_lines:
            lines = []
            for i, line in enumerate(f, 1): 
                if i > max_lines:
                    lines.append(f"... (truncated after {max_lines} lines)")
                    break
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

def run_executable(executable: str, input_content: str) -> str:
    try:
        result = subprocess.run(
                [executable],
                input=input_content,
                capture_output=True,
                text=True,
                check=True
                )
        return result.stdout
    except subprocess.CalledProcessError as e:
        return f"Error: Process failed with code {e.returncode}\n{e.stderr}"
    except FileNotFoundError:
        return f"Error: Executable '{executable}' not found"

def get_diff(actual: str, expected: str, width: int = 10) -> str:
    actual_lines = actual.splitlines()
    expected_lines = expected.splitlines()
    
    # Make both lists the same length by padding with empty strings
    max_len = max(len(actual_lines), len(expected_lines))
    actual_lines.extend([''] * (max_len - len(actual_lines)))
    expected_lines.extend([''] * (max_len - len(expected_lines)))
    
    # Calculate column widths (40% for left, 40% for right, 20% for middle marker)
    left_width = int(width * 0.4)
    right_width = int(width * 0.4)
    marker_width = width - left_width - right_width
    
    result = []
    result.append(" " * left_width + " " * marker_width + " " * right_width)
    result.append("LEFT".ljust(left_width) + " " * marker_width + "RIGHT".ljust(right_width))
    result.append("-" * left_width + "-" * marker_width + "-" * right_width)
    
    for actual_line, expected_line in zip(actual_lines, expected_lines):
        if actual_line == expected_line:
            marker = "  "  # Two spaces for matching lines
        elif actual_line == "":
            marker = "<<"
            expected_line = expected_line.ljust(right_width)
        elif expected_line == "":
            marker = ">>"
            actual_line = actual_line.ljust(left_width)
        else:
            marker = "! "
        
        # Truncate lines if they're too long
        if len(actual_line) > left_width:
            actual_line = actual_line[:left_width-3] + "..."
        if len(expected_line) > right_width:
            expected_line = expected_line[:right_width-3] + "..."
        
        # Format the line
        line = f"{actual_line:<{left_width}}{marker:<{marker_width}}{expected_line:<{right_width}}"
        result.append(line)
    
    return '\n'.join(result)

def run_test(executable: str, input_file: Path, expected_file: Path, max_input_lines: int = 10) -> bool:
    test_name = input_file.stem
    input_content_full = read_file_content(input_file)
    input_content_preview = read_file_content(input_file, max_input_lines)
    expected_output_with_numbers = read_file_content_with_line_numbers(expected_file)
    actual_output = run_executable(executable, input_content_full)

    if actual_output == read_file_content(expected_file):
        return True

    print(f"test name: {test_name}")
    print("===INPUT===")
    print(input_content_preview)
    print("===OUTPUTS===")
    actual_lines = actual_output.splitlines()
    numbered_actual = '\n'.join([f"{i+1:3d}: {line}" for i, line in enumerate(actual_lines)])
    print(numbered_actual)
    print("===EXPECTED (with line numbers)===")
    print(expected_output_with_numbers)
    print("===DIFFERENCE===")
    print(get_diff(actual_output, read_file_content(expected_file)))

    return False

def main():
    if len(sys.argv) != 2:
        print("Usage: python test_runner.py <executable>")
        sys.exit(1)

    executable = sys.argv[1]

    test_pairs = find_test_pairs()

    if not test_pairs:
        script_dir = Path(__file__).parent
        print(f"No test pairs (*.i and *.e) found in {script_dir}")
        sys.exit(0)

    print(f"Found {len(test_pairs)} test(s)")

    for i, (input_file, expected_file) in enumerate(test_pairs, 1):
        print(f"Running test {i}: {input_file.name}")
        if not run_test(executable, input_file, expected_file):
            print(f"\nTest failed: {input_file.stem}")
            sys.exit(1)

    print("\nAll tests passed!")
    sys.exit(0)

if __name__ == "__main__":
    main()
