#!/bin/bash
set -e

BUILD_TYPE="Debug"

while [[ $# -gt 0 ]]; do
    case $1 in
    --build-type)
        if [[ "$2" != "Debug" && "$2" != "Release" ]]; then
            echo "Error: --build-type must be Debug or Release"
            exit 1
        fi
        BUILD_TYPE="$2"
        shift 2
        ;;
    *)
        shift
        ;;
    esac
done

PROJECT_ROOT=$(dirname "$(readlink -f "$0")")
VENV_DIR="${PROJECT_ROOT}/.venv"
CMAKE_DIR="${PROJECT_ROOT}/cmake"

mkdir -p ${CMAKE_DIR}

if [ ! -d "${CMAKE_DIR}/CPM.cmake" ]; then
    echo "Downloading CPM.cmake..."
    wget -O cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
fi

if [ ! -d "${VENV_DIR}" ]; then
    echo "Setting up Python virtual environment..."
    python3 -m venv "${VENV_DIR}"
fi

if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    source "${VENV_DIR}/Scripts/activate"
else
    source "${VENV_DIR}/bin/activate"
fi

echo "Installing required Python packages..."
pip install -U pip
pip install -r "${PROJECT_ROOT}/scripts/codegen/requirements.txt"

echo "Configuring project with CMake..."
cmake --preset $(echo "${BUILD_TYPE}")

echo "Building project..."
cmake --build --preset $(echo "${BUILD_TYPE}")

echo "Setup complete. Project built successfully."