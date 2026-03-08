#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/build.sh
#   ./scripts/build.sh Debug
#   ./scripts/build.sh Release clean

CONFIG="${1:-Release}"
CLEAN="${2:-}"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"
INSTALL_DIR="${ROOT_DIR}/install"

echo "Root:   ${ROOT_DIR}"
echo "Build:  ${BUILD_DIR}"
echo "Config: ${CONFIG}"

if [[ "${CLEAN}" == "clean" ]]; then
    rm -rf "${BUILD_DIR}" "${INSTALL_DIR}"
    echo "Cleaned build/ and install/"
fi

mkdir -p "${BUILD_DIR}" "${INSTALL_DIR}"

CMAKE_ARGS=(
    -S "${ROOT_DIR}"
    -B "${BUILD_DIR}"
    -DCMAKE_BUILD_TYPE="${CONFIG}"
    -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}"
)

echo
echo "=== Configure ==="
cmake "${CMAKE_ARGS[@]}"

echo
echo "=== Build ==="
cmake --build "${BUILD_DIR}" -j

echo
echo "=== Install (optional) ==="
cmake --install "${BUILD_DIR}"

echo 
echo "Done."