#!/bin/bash
set -e

PROJECT_ROOT=$(dirname "$(readlink -f "$0")")
VENV_DIR="${PROJECT_ROOT}/.venv"
BUILD_DIR="${PROJECT_ROOT}/build"
CONAN_PY_FILE="${PROJECT_ROOT}/scripts/third_party/conanfile.py"

echo "Setting up Python virtual environment..."
python3 -m venv "${VENV_DIR}"

if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    source "${VENV_DIR}/Scripts/activate"
else
    source "${VENV_DIR}/bin/activate"
fi

echo "Installing required Python packages..."
pip install -U pip
pip install -r "${PROJECT_ROOT}/scripts/codegen/requirements.txt"
pip install -r "${PROJECT_ROOT}/scripts/third_party/requirements.txt"

echo "Installing C   dependencies with Conan..."
mkdir -p "${BUILD_DIR}"
conan install "${CONAN_PY_FILE}" --output-folder="${BUILD_DIR}" --build=missing

echo "Configuring project with CMake..."
cmake --preset default

echo "Building project..."
cmake --build --preset debug

echo "Setup complete. Project built successfully."