from .match import get_diff, number_lines
from .models import ExecResult, TestCase


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

