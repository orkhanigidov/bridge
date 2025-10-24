cmake_minimum_required(VERSION 3.28)

if (NOT CMAKE_BINARY_DIR)
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
endif ()

project(Codegen
        VERSION 1.0.0
        DESCRIPTION "Code generation tool for binding C++ code to Lua using Sol2"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CODEGEN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CODEGEN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CLANG_INCLUDE_DIR ${THIRD_PARTY_DIR}/clang/include)
add_library(clang::libclang UNKNOWN IMPORTED)

if (WIN32)
    message(STATUS "Windows platform detected. Using libclang from third_party directory.")

    set(LIBCLANG_LIB ${THIRD_PARTY_DIR}/clang/lib/libclang.lib)
    set(LIBCLANG_BIN ${THIRD_PARTY_DIR}/clang/bin/libclang.dll)
else ()
    message(STATUS "Linux platform detected. Searching for libclang on the system.")

    file(GLOB LIBCLANG_LIB_FOUND
            LIST_DIRECTORIES false
            "/usr/lib/llvm-*/lib/libclang.so.*"
            "/usr/lib/x86_64-linux-gnu/libclang.so.*"
    )

    list(GET LIBCLANG_LIB_FOUND 0 LIBCLANG_LIB)

    if (NOT LIBCLANG_LIB)
        message(FATAL_ERROR "Could not find any libclang.so on the system. Please install libclang development package.")
    endif ()
endif ()

set_target_properties(clang::libclang PROPERTIES
        IMPORTED_LOCATION ${LIBCLANG_LIB}
        INTERFACE_INCLUDE_DIRECTORIES ${CLANG_INCLUDE_DIR}
)

set(SOURCES
        ${CODEGEN_SRC_DIR}/analysis/utils/clang_utils.cpp
        ${CODEGEN_SRC_DIR}/analysis/ast_visitor.cpp
        ${CODEGEN_SRC_DIR}/analysis/clang_analyzer.cpp
        ${CODEGEN_SRC_DIR}/generation/sol2_generator.cpp
        ${CODEGEN_SRC_DIR}/io/env_reader.cpp
        ${CODEGEN_SRC_DIR}/io/header_collector.cpp
        ${CODEGEN_SRC_DIR}/io/yaml_reader.cpp
        ${CODEGEN_SRC_DIR}/engine.cpp
        ${CODEGEN_SRC_DIR}/main.cpp
)

add_executable(${PROJECT_NAME}
        ${SOURCES}
)

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBCLANG_BIN} $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying libclang.dll to build directory"
    )
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:${CODEGEN_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        yaml-cpp::yaml-cpp
        clang::libclang
        OGDF
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
        ${CODEGEN_INCLUDE_DIR}/pch.hpp
)

message(STATUS "Codegen project configuration completed successfully!")
message(STATUS " - Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS " - C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")