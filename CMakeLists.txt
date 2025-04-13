cmake_minimum_required(VERSION 3.30)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Load environment variables from .env file
set(ENV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/.env")
if (EXISTS "${ENV_FILE}")
    file(STRINGS "${ENV_FILE}" env_vars)
    foreach (env_var ${env_vars})
        if (env_var MATCHES "^[^#]" AND NOT env_var STREQUAL "")
            if (env_var MATCHES "^([^=]+)=(.*)$")
                set(var_name "${CMAKE_MATCH_1}")
                set(var_value "${CMAKE_MATCH_2}")
                set(${var_name} "${var_value}")
                message(STATUS "Loaded from .env: ${var_name}=${var_value}")
            endif ()
        endif ()
    endforeach ()
else ()
    message(WARNING "No .env file found. Copy .env.example to .env and configure your environment variables.")
endif ()

# Required variable check
foreach (required_var ROOT_DIR)
    if (NOT DEFINED "${required_var}")
        message(FATAL_ERROR "${required_var} is not defined. Please set it in the .env file.")
    endif ()
endforeach ()

add_executable(Engine src/main.cpp
        include/ParameterConverter.h
        include/ResultConverter.h
        include/Registry.h
        include/Registry.inl
        include/GeneratedRegistry.h
        src/Registry.cpp
        src/Globals.cpp
)

# Default values for optional variables
if (NOT DEFINED INCLUDE_DIR)
    set(INCLUDE_DIR "include")
endif ()
if (NOT DEFINED BUILD_DIR)
    set(BUILD_DIR "build")
endif ()

# Define Python virtual environment paths
set(VENV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.venv)
set(VENV_PYTHON ${CMAKE_CURRENT_SOURCE_DIR}/.venv/Scripts/python${CMAKE_EXECUTABLE_SUFFIX})
set(VENV_PIP ${CMAKE_CURRENT_SOURCE_DIR}/.venv/Scripts/pip${CMAKE_EXECUTABLE_SUFFIX})

# Find Python interpreter
find_package(Python COMPONENTS Interpreter REQUIRED)

# Setup Python virtual environment if it doesn't exist yet
set(VENV_MARKER ${VENV_DIR}/.initialized)
add_custom_command(
        OUTPUT ${VENV_MARKER}
        COMMAND ${CMAKE_COMMAND} -E echo "Setting up Python virtual environment..."
        COMMAND ${CMAKE_COMMAND} -E make_directory ${VENV_DIR}
        COMMAND ${Python_EXECUTABLE} -m venv ${VENV_DIR}
        COMMAND ${CMAKE_COMMAND} -E echo "Installing required Python packages..."
        COMMAND ${VENV_PIP} install --upgrade pip
        COMMAND ${VENV_PIP} install libclang
        COMMAND ${CMAKE_COMMAND} -E touch ${VENV_MARKER}
        COMMENT "Setting up Python environment"
        VERBATIM
)

# Create directory for generated source files
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/generated)

# Registry code generation
# This command runs generate_registry.py to create the implementation file whenever the script or configuration file changes
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/generated/GeneratedRegistry.cpp
        COMMAND ${CMAKE_COMMAND} -E echo "Generating registry code..."
        COMMAND ${VENV_PYTHON} ${CMAKE_CURRENT_SOURCE_DIR}/generate_registry.py > ${CMAKE_CURRENT_SOURCE_DIR}/src/generated/GeneratedRegistry.cpp
        DEPENDS ${VENV_MARKER} ${CMAKE_CURRENT_SOURCE_DIR}/generate_registry.py ${CMAKE_CURRENT_SOURCE_DIR}/config/registry_config.toml
        COMMENT "Generating registry implementation"
        VERBATIM
)

# Add the generated implementation to the build
target_sources(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/generated/GeneratedRegistry.cpp)

# Use file(TO_CMAKE_PATH) for platform-independent path conversion
file(TO_CMAKE_PATH "${ROOT_DIR}/${INCLUDE_DIR}" INCLUDE_DIR)
target_include_directories(Engine PRIVATE "${INCLUDE_DIR}")

if (DEFINED LIBRARIES)
    string(REPLACE ";" " " libs "${LIBRARIES}")
    separate_arguments(libs)
    foreach (lib ${libs})
        file(TO_CMAKE_PATH "${ROOT_DIR}/${BUILD_DIR}/${lib}" LIB_PATH)
        if (NOT EXISTS "${LIB_PATH}")
            message(WARNING "Library '${LIB_PATH}' not found.")
        endif ()
        target_link_libraries(Engine PRIVATE "${LIB_PATH}")
    endforeach ()
endif ()