import re
from dataclasses import dataclass
from pathlib import Path

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

MIN_OUTPUT_LIMIT_BYTES = 4096
MAX_OUTPUT_LIMIT_BYTES = 256 * 1024
MAX_RUNTIME_SECONDS = 1.0
ERROR_SLACK_BYTES = 200
OUTPUT_MARGIN_BYTES = 1024


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

