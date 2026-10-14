#!/bin/bash
set -euo pipefail

BUILD_TYPE="Debug"
INSTALL_DEPS=false
BUILD_SHARED=false
INSTALL_SERVICE=false

print_error() {
  echo "[$(date -u +'%Y-%m-%dT%H:%M:%S%z')]: $*" >&2
}

show_usage() {
  echo "Usage: $0 [--build-type Debug|Release] [--install-deps] [--build-shared] [-h|--help]" >&2
  echo ""
  echo "Options:"
  echo "  --build-type <TYPE>          Specify the build type (Debug or Release). Default is Debug."
  echo "  --install-deps               Attempt to install required dependencies. Requires root privileges."
  echo "  --build-shared               Build the engine as shared library."
  echo "  --install-service Install and enable a systemd auto-restart service. Requires root."
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
    PACKAGES="g++ cmake lua5.4 liblua5.4-dev libclang1-18 ninja-build python3-full"
    INSTALL_CMD="apt-get update && apt-get install -y ${PACKAGES}"
  elif command -v yum &> /dev/null; then
    PKG_MANAGER="yum"
    PACKAGES="gcc-c++ cmake lua5.4 liblua5.4-dev libclang1-18 ninja-build"
    INSTALL_CMD="yum install -y ${PACKAGES}"
  elif command -v dnf &> /dev/null; then
    PKG_MANAGER="dnf"
    PACKAGES="gcc-c++ cmake lua5.4 liblua5.4-dev libclang1-18 ninja-build"
    INSTALL_CMD="dnf install -y ${PACKAGES}"
  else
    print_error "Unsupported package manager. Cannot install dependencies automatically."
    print_error "Please install manually: C/C++ compiler, CMake, Lua, Ninja, Clang."
    exit 1
  fi

  echo "Using ${PKG_MANAGER} to install packages: ${PACKAGES}"
  if ! eval "${INSTALL_CMD}"; then
    print_error "Failed to install dependencies."
    exit 1
  fi
  echo "Dependencies installed successfully."
}

install_service() {
  if [[ $EUID -ne 0 ]]; then
    print_error "Service installation requires root privileges."
    print_error "Please run again with 'sudo $0 --install-service'"
    exit 1
  fi
  if [[ "${BUILD_SHARED}" = true ]]; then
    print_error "Service is only for the server (executable) build, not --build-shared."
    exit 1
  fi

  local SCRIPT_DIR
  SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  local SERVICE_USER="${SUDO_USER:-root}"
  local ENGINE_BIN="${SCRIPT_DIR}/build/Engine"
  local SERVICE_FILE="/etc/systemd/system/bridge-engine.service"

  if [[ ! -f "${ENGINE_BIN}" ]]; then
    print_error "Engine binary not found at ${ENGINE_BIN}. Build the project first."
    exit 1
  fi

  echo "Installing systemd service (${SERVICE_FILE})..."
  cat > "${SERVICE_FILE}" <<EOF
[Unit]
Description=BRIDGE Engine REST API Server
After=network.target
StartLimitIntervalSec=0

[Service]
Type=simple
User=${SERVICE_USER}
WorkingDirectory=${SCRIPT_DIR}
ExecStart=${ENGINE_BIN} --host 0.0.0.0 --port 8000
Restart=always
RestartSec=2

[Install]
WantedBy=multi-user.target
EOF

    systemctl daemon-reload
    systemctl enable bridge-engine.service
    systemctl restart bridge-engine.service
    echo "Service installed and started. Check with: systemctl status bridge-engine"
}

while [[ $# -gt 0 ]]; do
    case "$1" in
    --build-type)
        if [[ $# -lt 2 ]]; then
            print_error "Missing argument for --build-type"
            show_usage
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
    --build-shared)
      BUILD_SHARED=true
      shift 1
      ;;
    --install-service)
      INSTALL_SERVICE=true
      shift 1
      ;;
    -h|--help)
        show_usage
        ;;
    *)
      print_error "Unknown argument: $1"
      show_usage
      ;;
    esac
done

if [[ "${INSTALL_SERVICE}" = true && "${INSTALL_DEPS}" = false ]]; then
  install_service
  exit 0
fi

if [[ "${INSTALL_DEPS}" = true ]]; then
  install_dependencies
fi

cmakeConfigureArgs=()
cmakeConfigureArgs+=(--preset "${BUILD_TYPE}")

if [[ "${BUILD_SHARED}" = true ]]; then
  echo "Configuring to build as a SHARED library."
  cmakeConfigureArgs+=("-DBUILD_SHARED_LIB:BOOL=ON")
else
  echo "Configuring to build as an EXECUTABLE."
  cmakeConfigureArgs+=("-DBUILD_SHARED_LIB:BOOL=OFF")
fi

echo "Configuring project (cmake ${cmakeConfigureArgs[*]})..."
cmake "${cmakeConfigureArgs[@]}" || {
  rc=$?
  print_error "CMake configure failed with exit code ${rc}."
  exit "${rc}"
}

echo "Building project..."
if cmake --build --preset "${BUILD_TYPE}"; then
  echo "Build completed successfully."
else
  rc=$?
  print_error "Build failed with exit code ${rc}."
  exit ${rc}
fi

if [[ "${INSTALL_SERVICE}" = true ]]; then
  install_service
fi