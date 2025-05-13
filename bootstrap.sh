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
BUILD_DIR="${PROJECT_ROOT}/build"
CONAN_PY_FILE="${PROJECT_ROOT}/scripts/third_party/conanfile.py"

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
pip install -r "${PROJECT_ROOT}/scripts/third_party/requirements.txt"

if ! conan profile list | grep -q "default"; then
    conan profile detect
fi

echo "Installing C++ dependencies with Conan..."
mkdir -p "${BUILD_DIR}"
conan install "${CONAN_PY_FILE}" --output-folder="${BUILD_DIR}" --build=missing --settings=build_type=${BUILD_TYPE}

echo "Configuring project with CMake..."
cmake --preset $(echo "${BUILD_TYPE}" | tr '[:upper:]' '[:lower:]')

echo "Building project..."
cmake --build --preset $(echo "${BUILD_TYPE}" | tr '[:upper:]' '[:lower:]')

echo "Setup complete. Project built successfully."