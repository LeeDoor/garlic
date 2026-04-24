import re
from itertools import zip_longest

from .models import (
    ANSI_BOLD,
    ANSI_GREEN,
    ANSI_RED,
    ANSI_RESET,
    ANSI_YELLOW,
    ERROR_WILDCARDS,
    LOCATION_ERROR_PREFIX_RE,
)


def normalize_lines(content: str) -> list[str]:
    return content.splitlines()


def number_lines(text: str) -> str:
    return "\n".join(f"{index:3d}: {line}" for index, line in enumerate(normalize_lines(text), 1))


def strip_cli_prompt_prefix(line: str) -> str:
    while line.startswith("#>"):
        line = line[2:]
        if line.startswith(" "):
            line = line[1:]
    return line


def prepare_actual_lines(mode: str, actual_output: str) -> list[str]:
    lines = normalize_lines(actual_output)
    if mode != "location_cli":
        return lines

    prepared = [strip_cli_prompt_prefix(line) for line in lines]
    while prepared and prepared[0] == "":
        prepared.pop(0)
    return prepared


def prepare_expected_lines(mode: str, expected_output: str) -> list[str]:
    lines = normalize_lines(expected_output)
    if mode != "location_cli":
        return lines
    return [re.sub(r"^(#> ?)+", "", line) for line in lines]


def line_matches(expected_line: str, actual_line: str, mode: str) -> bool:
    if mode in {"location", "location_cli"} and LOCATION_ERROR_PREFIX_RE.match(expected_line):
        return actual_line.startswith(expected_line)

    checker = ERROR_WILDCARDS.get(expected_line)
    if checker is not None:
        return checker(actual_line)

    return expected_line == actual_line


def outputs_match(expected_output: str, actual_output: str, mode: str) -> bool:
    expected_lines = prepare_expected_lines(mode, expected_output)
    actual_lines = prepare_actual_lines(mode, actual_output)
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
    expected_lines = prepare_expected_lines(mode, expected_output)
    missing = object()

    marker_width = 8
    content_width = max(40, width - marker_width)
    left_width = content_width // 2
    right_width = content_width - left_width

    result = [
        " " * left_width + " " * marker_width + " " * right_width,
        style("ACTUAL", ANSI_BOLD).ljust(left_width) + " " * marker_width + style("EXPECTED", ANSI_BOLD).ljust(right_width),
        "-" * left_width + "-" * marker_width + "-" * right_width,
    ]

    for actual_line, expected_line in zip_longest(actual_lines, expected_lines, fillvalue=missing):
        if expected_line is missing:
            marker = style(" >> ", ANSI_BOLD, ANSI_YELLOW)
            actual_text = actual_line
            expected_text = "<no line>"
        elif actual_line is missing:
            marker = style(" << ", ANSI_BOLD, ANSI_YELLOW)
            actual_text = "<no line>"
            expected_text = expected_line
        elif line_matches(expected_line, actual_line, mode):
            marker = style("~OK~", ANSI_BOLD, ANSI_GREEN) if expected_line in ERROR_WILDCARDS else style(" OK ", ANSI_GREEN)
            actual_text = actual_line
            expected_text = expected_line
        else:
            marker = style(" !! ", ANSI_BOLD, ANSI_RED)
            actual_text = actual_line
            expected_text = expected_line

        result.append(
            f"{truncate_for_width(actual_text, left_width):<{left_width}} "
            f"{marker} "
            f"{truncate_for_width(expected_text, right_width):<{right_width}}"
        )

    return "\n".join(result)

