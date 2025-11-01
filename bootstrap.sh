#!/bin/bash
set -euo pipefail

BUILD_TYPE="Debug"
INSTALL_DEPS=false

print_error() {
  echo "[$(date -u +'%Y-%m-%dT%H:%M:%S%z')]: $*" >&2
}

usage() {
  echo "Usage: $0 [--build-type Debug|Release] [--install-deps] [-h|--help]" >&2
  echo ""
  echo "Options:"
  echo "  --build-type <TYPE>          Specify the build type (Debug or Release). Default is Debug."
  echo "  --install-deps               Attempt to install required dependencies. Requires root privileges."
  echo "  -h, --help                   Show this help message."
  exit 1
}

install_dependencies(){
  if [[ $EUID -ne 0 ]]; then
    print_error "Dependency installation requires root privileges."
    print_error "Please run again with 'sudo $0 --install-deps ...' or install dependencies manually."
    exit 1
  fi

  echo "Attempting to install required dependencies..."
  local PKG_MANAGER=""
  local INSTALL_CMD=""
  local PACKAGES=""

  if command -v apt-get &> /dev/null; then
    PKG_MANAGER="apt"
    PACKAGES="build-essential g++ cmake lua5.4 liblua5.4-dev libclang1-20 ninja-build wget curl"
    INSTALL_CMD="apt-get update && apt-get install -y ${PACKAGES}"
  elif command -v yum &> /dev/null; then
    PKG_MANAGER="yum"
    PACKAGES="gcc-c++ cmake lua5.4 liblua5.4-dev libclang1-20 ninja-build wget curl"
    INSTALL_CMD="yum install -y ${PACKAGES}"
  elif command -v dnf &> /dev/null; then
    PKG_MANAGER="dnf"
    PACKAGES="gcc-c++ cmake lua5.4 liblua5.4-dev libclang1-20 ninja-build wget curl"
    INSTALL_CMD="dnf install -y ${PACKAGES}"
  else
    print_error "Unsupported package manager. Cannot install dependencies automatically."
    print_error "Please install manually: C/C++ compiler, CMake, Lua, Ninja, wget, curl"
    exit 1
  fi

  echo "Using ${PKG_MANAGER} to install packages: ${PACKAGES}"
  if ! eval "${INSTALL_CMD}"; then
    print_error "Failed to install dependencies."
    exit 1
  fi
  echo "Dependencies installed successfully."
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
    --install-deps)
      INSTALL_DEPS=true
      shift 1
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

if [[ "${INSTALL_DEPS}" = true ]]; then
  install_dependencies
fi

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CMAKE_DIR="${PROJECT_ROOT}/cmake"

mkdir -p "${CMAKE_DIR}"

if [[ ! -f "${CMAKE_DIR}/CPM.cmake" ]]; then
    echo "Downloading CPM.cmake..."
    CPM_URL="https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake"
    if command -v wget &> /dev/null; then
      wget -q -O "${CMAKE_DIR}/CPM.cmake" "${CPM_URL}"
    elif command -v curl &> /dev/null; then
      curl -sSL -o "${CMAKE_DIR}/CPM.cmake" "${CPM_URL}"
    else
      print_error "Neither wget nor curl is available to download CPM.cmake."
      exit 1
    fi
fi

echo "Configuring project..."
cmake --preset "${BUILD_TYPE}"

echo "Building project..."
cmake --build --preset "${BUILD_TYPE}"

echo "Build completed successfully."