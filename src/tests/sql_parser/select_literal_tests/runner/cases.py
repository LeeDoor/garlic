import re
import sys
from pathlib import Path
from typing import Iterable

from .models import SPECIAL_MODES, TestCase


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


def summarize_case_counts(cases: list[TestCase]) -> str:
    counts: dict[str, int] = {}
    for case in cases:
        counts[case.group] = counts.get(case.group, 0) + 1
    return ", ".join(f"{count} {group} test(s)" for group, count in counts.items())

