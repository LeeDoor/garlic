#!/usr/bin/env python3
from pathlib import Path
import runpy

if __name__ == "__main__":
    new_runner = Path(__file__).parent / "select_literal_tests" / "input_output_comparator.py"
    runpy.run_path(str(new_runner), run_name="__main__")
