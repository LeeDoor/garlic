#!/usr/bin/env bash
clear
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"
EXE_REL="src/garlic"

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"
rm -f "${EXE_REL}"
cmake "${ROOT_DIR}"
cmake --build . -j"$(nproc)"
