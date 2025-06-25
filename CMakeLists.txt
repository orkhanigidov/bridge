cmake_minimum_required(VERSION 3.25)
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
set(REFLECTION_CONFIG ${PROJECT_ROOT}/scripts/codegen/config/reflection_config.yaml)
set(REFLECTION_OUTPUT ${GENERATED_DIR}/SolGenerated.cpp)

# Create directory for generated source files
file(MAKE_DIRECTORY ${GENERATED_DIR})

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        VERSION 3.3.0
)
CPMAddPackage(
        NAME oatpp
        GIT_REPOSITORY https://github.com/oatpp/oatpp.git
        GIT_TAG 1.3.1
)
CPMAddPackage(
        NAME ogdf
        GIT_REPOSITORY https://github.com/ogdf/ogdf.git
        GIT_TAG elderberry-202309
)

#find_package(Python COMPONENTS Interpreter REQUIRED)

#if (WIN32)
#    set(PYTHON_EXE ${PROJECT_ROOT}/.venv/Scripts/python.exe)
#else ()
#    set(PYTHON_EXE ${PROJECT_ROOT}/.venv/bin/python)
#endif ()

# Registry code generation
# This command runs main.py to create the implementation file whenever the script or configuration file changes
#add_custom_command(
#        OUTPUT ${REFLECTION_OUTPUT}
#        COMMAND ${CMAKE_COMMAND} -E echo "Generating registry code..."
#        COMMAND ${PYTHON_EXE} ${PROJECT_ROOT}/scripts/codegen/main.py
#        --config=${REFLECTION_CONFIG}
#        --package=ogdf
#        --output=${REFLECTION_OUTPUT}
#        --verbose
#        DEPENDS ${PROJECT_ROOT}/scripts/codegen/main.py ${REFLECTION_CONFIG}
#        VERBATIM
#)

file(GLOB_RECURSE HEADERS ${INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${REFLECTION_OUTPUT})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

find_package(Lua REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})

target_link_libraries(${PROJECT_NAME} PRIVATE sol2::sol2 oatpp OGDF COIN)

# Add precompiled header configuration
target_precompile_headers(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR}/pch.hpp)

# Set compiler warnings
#if (MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
#else ()
#    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
#endif ()