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

set(LUA_INCLUDE_DIR ${PROJECT_ROOT}/external/lua/include)
set(LUA_LIBRARY ${PROJECT_ROOT}/external/lua/lua54.lib)

set(CLANG_INCLUDE_DIR ${PROJECT_ROOT}/external/clang/include)
set(CLANG_LIBRARY ${PROJECT_ROOT}/external/clang/libclang.lib)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 0.8.0
)
CPMAddPackage(
        NAME sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        VERSION 3.5.0
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
        OPTIONS
        "OGDF_DEBUG ON"
        "OGDF_USE_ASSERT_EXCEPTIONS ON"
        "OGDF_USE_ASSERT_EXCEPTIONS_WITH_STACK_TRACE ON"
        "OGDF_MEMORY_MALLOC_TS ON"
)

file(GLOB_RECURSE HEADERS ${INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCES}
        ${GENERATED_OUTPUT}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${INCLUDE_DIR}
        ${LUA_INCLUDE_DIR}
        ${CLANG_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        yaml-cpp::yaml-cpp
        sol2::sol2
        oatpp
        OGDF
        ${LUA_LIBRARY}
        ${CLANG_LIBRARY}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR}/pch.hpp)