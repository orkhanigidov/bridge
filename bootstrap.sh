#!/bin/bash
set -euo pipefail

BUILD_TYPE="Debug"

print_error() {
  echo "[$(date + '%Y-%m-%dT%H:%M:%S%z')]: $*" >&2
}

usage() {
  echo "Usage: $0 [--build-type Debug|Release]" >&2
  exit 1
}

while [[ $# -gt 0 ]]; do
    case "$1" in
    --build-type)
        if [[ $# -lt 2 ]]; then
            print_error "Missing argument for --build-type"
            usage
        fi
        if [[ "$2" != "Debug" && "$2" != "Release" ]]; then
            print_error "--build-type must be Debug or Release"
            exit 1
        fi
        BUILD_TYPE="$2"
        shift 2
        ;;
    -h|--help)
        usage
        ;;
    *)
      print_error "Unknown argument: $1"
      usage
      ;;
    esac
done

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VENV_DIR="${PROJECT_ROOT}/.venv"
CMAKE_DIR="${PROJECT_ROOT}/cmake"

mkdir -p "${CMAKE_DIR}"

if [[ ! -f "${CMAKE_DIR}/CPM.cmake" ]]; then
    echo "Downloading CPM.cmake..."
    wget -q -O "${CMAKE_DIR}/CPM.cmake" https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
fi

if [[ ! -d "${VENV_DIR}" ]]; then
    echo "Setting up Python virtual environment..."
    python3 -m venv "${VENV_DIR}"
fi

if [[ "${OSTYPE}" == "msys" || "${OSTYPE}" == "win32" ]]; then
    source "${VENV_DIR}/Scripts/activate"
else
    source "${VENV_DIR}/bin/activate"
fi

echo "Installing required Python packages..."
pip install -U pip
pip install -r "${PROJECT_ROOT}/scripts/codegen/requirements.txt"

echo "Configuring project with CMake..."
cmake --preset "${BUILD_TYPE}"

echo "Building project..."
cmake --build --preset "${BUILD_TYPE}"

echo "Setup complete. Project built successfully."