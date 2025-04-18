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

add_library(Engine SHARED
        include/ParameterConverter.h
        include/ResultConverter.h
        include/TypeRegistry.h
        include/CustomTypeTraits.h
        include/Registry.h
        include/Registry.inl
        include/GeneratedRegistry.h
        include/UnityBridge.h
        src/UnityBridge.cpp
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
        COMMAND ${VENV_PIP} install toml
        COMMAND ${CMAKE_COMMAND} -E touch ${VENV_MARKER}
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

# Set Unity Plugins directory from environment or use default
if (NOT DEFINED UNITY_PROJECT_DIR)
    set(UNITY_PLUGIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/UnityPlugin)
    message(STATUS "Using default Unity Plugin directory: ${UNITY_PLUGIN_DIR}")
else ()
    message(STATUS "Using Unity Project directory from .env: ${UNITY_PROJECT_DIR}")
endif ()

if (NOT DEFINED DLL_OUTPUT_NAME)
    set(DLL_OUTPUT_NAME "EngineBridge")
endif ()

set_target_properties(Engine PROPERTIES
        PREFIX ""  # Remove "lib" prefix from DLL
        OUTPUT_NAME ${DLL_OUTPUT_NAME}
)

# Post-build command to copy DLL to Unity Plugins directory
add_custom_command(TARGET Engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying DLL to Unity Plugins directory..."
        COMMAND ${CMAKE_COMMAND} -E make_directory ${UNITY_PROJECT_DIR}/Assets/Plugins
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Engine> ${UNITY_PLUGINS_PATH}/$<TARGET_FILE_NAME:Engine>
        VERBATIM
)

# Disable specific warnings for MSVC
if (MSVC)
    target_compile_options(Engine PRIVATE /wd4068)  # Disable unknown pragma 'mark' warning
endif ()