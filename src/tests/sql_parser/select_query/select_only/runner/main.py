import sys
from pathlib import Path

from .cases import collect_cases, summarize_case_counts
from .match import outputs_match
from .process import run_executable
from .report import report_failure


def run_case(executable: str, case) -> bool:
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
    root = Path(__file__).resolve().parent.parent
    cases = collect_cases(root)

    if not cases:
        print(f"No test files found in {root}")
        sys.exit(0)

    print("Found " + summarize_case_counts(cases))

    running: dict[str, int] = {}
    for case in cases:
        running[case.group] = running.get(case.group, 0) + 1
        print(f"Running {case.group} test {running[case.group]}: {case.input_file.name}")
        if not run_case(executable, case):
            print(f"\nTest failed: {case.input_file.stem}")
            sys.exit(1)

    print("\nAll tests passed!")

