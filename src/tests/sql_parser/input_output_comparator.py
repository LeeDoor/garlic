import os
import sys
import subprocess
import difflib
from pathlib import Path
from typing import List, Tuple, Optional

def find_test_pairs() -> List[Tuple[Path, Path]]:
    script_dir = Path(__file__).parent
    i_files = sorted(script_dir.glob("*.dat"))
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
            for i, line in enumerate(f):
                if i >= max_lines:
                    lines.append(f"... (truncated after {max_lines} lines)")
                    break
                lines.append(line.rstrip('\n'))
            return '\n'.join(lines)
        else:
            return f.read()

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

def get_diff(actual: str, expected: str) -> str:
    actual_lines = actual.splitlines(keepends=True)
    expected_lines = expected.splitlines(keepends=True)

    diff = difflib.unified_diff(
            expected_lines,
            actual_lines,
            fromfile='expected',
            tofile='actual',
            n=3
            )
    return ''.join(diff)

def run_test(executable: str, input_file: Path, expected_file: Path, max_input_lines: int = 10) -> bool:
    test_name = input_file.stem
    input_content_full = read_file_content(input_file)
    input_content_preview = read_file_content(input_file, max_input_lines)
    expected_output = read_file_content(expected_file)
    actual_output = run_executable(executable, input_content_full)

    if actual_output == expected_output:
        return True

    print(f"test name: {test_name}")
    print("===INPUT===")
    print(input_content_preview)
    print("===OUTPUTS===")
    print(actual_output, end='' if actual_output.endswith('\n') else '\n')
    print("===EXPECTED===")
    print(expected_output, end='' if expected_output.endswith('\n') else '\n')
    print("===DIFFERENCE===")
    print(get_diff(actual_output, expected_output))

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

