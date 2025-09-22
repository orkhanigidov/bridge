cmake_minimum_required(VERSION 3.31)

if (NOT CMAKE_BINARY_DIR)
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Codegen
        VERSION 1.0.0
        DESCRIPTION ""
        LANGUAGES CXX)

set(CODEGEN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CODEGEN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    ${CODEGEN_SRC_DIR}/clang_analyzer.cpp
    ${CODEGEN_SRC_DIR}/engine.cpp
    ${CODEGEN_SRC_DIR}/env_reader.cpp
    ${CODEGEN_SRC_DIR}/header_collector.cpp
    ${CODEGEN_SRC_DIR}/sol2_generator.cpp
    ${CODEGEN_SRC_DIR}/yaml_reader.cpp
    ${CODEGEN_SRC_DIR}/main.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CODEGEN_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${INCLUDE_DIR}>
    $<BUILD_INTERFACE:${LUA_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${CLANG_INCLUDE_DIR}>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    yaml-cpp::yaml-cpp
    ${LIBCLANG_LIB}
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
        ${CODEGEN_INCLUDE_DIR}/pch.hpp
)

message(STATUS "")
message(STATUS "")