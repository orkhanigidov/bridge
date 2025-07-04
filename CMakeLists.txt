cmake_minimum_required(VERSION 3.25)
project(Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_DIR ${PROJECT_ROOT}/src)
set(INCLUDE_DIR ${PROJECT_ROOT}/include)
set(GENERATED_DIR ${SOURCE_DIR}/lua_bindings/generated)
set(CONFIG ${PROJECT_ROOT}/tools/codegen/config/bindings.yaml)
set(GENERATED_OUTPUT ${GENERATED_DIR}/generated_bindings.cpp)

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

find_package(Python REQUIRED)

if (WIN32)
    set(PYTHON_EXE ${PROJECT_ROOT}/.venv/Scripts/python.exe)
else ()
    set(PYTHON_EXE ${PROJECT_ROOT}/.venv/bin/python)
endif ()

add_custom_command(
        OUTPUT ${GENERATED_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
        COMMAND ${CMAKE_COMMAND} -E echo "Generating code..."
        COMMAND ${PYTHON_EXE} ${PROJECT_ROOT}/tools/codegen/cli.py
        --config=${CONFIG}
        --package=ogdf
        --output=${GENERATED_OUTPUT}
        --verbose
        DEPENDS ${PROJECT_ROOT}/tools/codegen/cli.py ${CONFIG}
        VERBATIM
)

find_package(Lua REQUIRED)

file(GLOB_RECURSE HEADERS ${INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCES}
        ${GENERATED_OUTPUT})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${INCLUDE_DIR}
        ${LUA_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        sol2::sol2
        oatpp
        OGDF
        ${LUA_LIBRARIES}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR}/pch.hpp)