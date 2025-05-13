cmake_minimum_required(VERSION 3.28)
project(Engine LANGUAGES CXX)

# Set C++ standard for all targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define project structure
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_DIR ${PROJECT_ROOT}/src)
set(INCLUDE_DIR ${PROJECT_ROOT}/include)
set(GENERATED_DIR ${SOURCE_DIR}/generated)
set(TEMPLATES_DIR ${PROJECT_ROOT}/templates)
set(REFLECTION_CONFIG ${PROJECT_ROOT}/config/reflection_config.yaml)
set(REFLECTION_TEMPLATE ${PROJECT_ROOT}/templates/reflection_registration_template.j2)
set(REFLECTION_OUTPUT ${GENERATED_DIR}/ReflectionRegistration.cpp)

# Create directory for generated source files
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Find required dependencies
find_package(Python COMPONENTS Interpreter REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(rttr CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)
find_package(ogdf CONFIG REQUIRED)

if (WIN32)
    set(PYTHON_EXE ${PROJECT_ROOT}/.venv/Scripts/python.exe)
else ()
    set(PYTHON_EXE ${PROJECT_ROOT}/.venv/bin/python)
endif ()

# Registry code generation
# This command runs generate_registry.py to create the implementation file whenever the script or configuration file changes
add_custom_command(
        OUTPUT ${REFLECTION_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E echo "Generating registry code..."
        COMMAND ${PYTHON_EXE} ${PROJECT_ROOT}/scripts/codegen/generate_registry.py
        --config=${REFLECTION_CONFIG}
        --templates=${TEMPLATES_DIR}
        --output=${REFLECTION_OUTPUT}
        DEPENDS ${PROJECT_ROOT}/scripts/codegen/generate_registry.py ${REFLECTION_CONFIG} ${REFLECTION_TEMPLATE}
        VERBATIM
)

file(GLOB_RECURSE HEADERS ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${REFLECTION_OUTPUT})

target_link_libraries(${PROJECT_NAME} PRIVATE
        nlohmann_json::nlohmann_json
        RTTR::Core_Lib
        cppzmq
        ogdf::ogdf
)

# Add precompiled header configuration
target_precompile_headers(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR}/pch.h)

# Set compiler warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif ()