import os
import select
import subprocess
import time

from .match import normalize_lines
from .models import (
    ERROR_SLACK_BYTES,
    ERROR_WILDCARDS,
    ExecResult,
    LOCATION_ERROR_PREFIX_RE,
    MAX_OUTPUT_LIMIT_BYTES,
    MAX_RUNTIME_SECONDS,
    MIN_OUTPUT_LIMIT_BYTES,
    OUTPUT_MARGIN_BYTES,
)


def estimate_output_limit(expected_output: str) -> int:
    lines = normalize_lines(expected_output)
    expected_chars = sum(len(line) + 1 for line in lines)
    error_lines = sum(
        1
        for line in lines
        if line in ERROR_WILDCARDS or LOCATION_ERROR_PREFIX_RE.match(line)
    )
    calculated = expected_chars + error_lines * ERROR_SLACK_BYTES + OUTPUT_MARGIN_BYTES
    return max(MIN_OUTPUT_LIMIT_BYTES, min(calculated, MAX_OUTPUT_LIMIT_BYTES))


def run_executable(
    executable: str,
    input_content: str,
    expected_output: str,
    extra_args: tuple[str, ...],
) -> ExecResult:
    max_output_bytes = estimate_output_limit(expected_output)

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
        if elapsed > MAX_RUNTIME_SECONDS:
            timed_out = True
            process.terminate()
            break
        if len(output) >= max_output_bytes:
            output_limited = True
            process.terminate()
            break
        if process.poll() is not None:
            break

        wait_timeout = min(0.05, max(0.0, MAX_RUNTIME_SECONDS - elapsed))
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

