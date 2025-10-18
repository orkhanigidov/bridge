cmake_minimum_required(VERSION 3.31)

if (NOT CMAKE_BINARY_DIR)
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
endif ()

project(Engine
        VERSION 1.0.0
        DESCRIPTION "C++ Engine with Lua Scripting Support"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIB "Build the engine as a shared library" OFF)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)
set(GENERATED_SRC ${CMAKE_BINARY_DIR}/generated_bindings.cpp)

set(CLANG_INCLUDE_DIR ${THIRD_PARTY_DIR}/clang/include)
set(LIBCLANG_LIB ${THIRD_PARTY_DIR}/clang/lib/libclang.lib)
set(LUA_INCLUDE_DIR ${THIRD_PARTY_DIR}/lua/include)
set(LUA_LIB ${THIRD_PARTY_DIR}/lua/lib/lua54.lib)

include(cmake/CPM.cmake)

CPMAddPackage("gh:jbeder/yaml-cpp#0.8.0")
CPMAddPackage("gh:ThePhD/sol2#develop")
CPMAddPackage("gh:oatpp/oatpp#1.3.1")
CPMAddPackage(
        NAME ogdf
        GIT_REPOSITORY https://github.com/ogdf/ogdf.git
        GIT_TAG elderberry-202309
        OPTIONS "OGDF_MEMORY_MALLOC_TS ON"
)

add_library(clang::libclang UNKNOWN IMPORTED)
set_target_properties(clang::libclang PROPERTIES
        IMPORTED_LOCATION ${LIBCLANG_LIB}
        INTERFACE_INCLUDE_DIRECTORIES ${CLANG_INCLUDE_DIR}
)

add_library(lua::lua54 UNKNOWN IMPORTED)
set_target_properties(lua::lua54 PROPERTIES
        IMPORTED_LOCATION ${LUA_LIB}
        INTERFACE_INCLUDE_DIRECTORIES ${LUA_INCLUDE_DIR}
)

add_subdirectory(tools/codegen)

set(SOURCES
        ${SRC_DIR}/bindings/lua/lua_binder.cpp
        ${SRC_DIR}/conversion/oatpp_type_adapter.cpp
        ${SRC_DIR}/execution/script/script_executor.cpp
        ${SRC_DIR}/execution/execution_engine.cpp
        ${SRC_DIR}/execution/execution_service.cpp
        ${SRC_DIR}/interop/interop_bridge.cpp
        ${SRC_DIR}/interop/interop_c_api.cpp
        ${SRC_DIR}/network/server_config.cpp
        ${SRC_DIR}/network/server_manager.cpp
        ${SRC_DIR}/utils/filesystem_utils.cpp
        ${SRC_DIR}/utils/response_factory.cpp
        ${SRC_DIR}/utils/string_utils.cpp
        ${SRC_DIR}/main.cpp
        ${GENERATED_SRC}
)

add_custom_command(
        OUTPUT ${GENERATED_SRC}
        COMMAND $<TARGET_FILE:Codegen>
        DEPENDS Codegen
        COMMENT "Generating Lua bindings from ${GENERATED_SRC}"
        VERBATIM
)

add_custom_target(generate_bindings DEPENDS ${GENERATED_SRC})

if (BUILD_SHARED_LIB)
    add_library(${PROJECT_NAME} SHARED
            ${SOURCES}
    )
    if (WIN32)
        target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_BUILDING_DLL)
    endif ()
else ()
    add_executable(${PROJECT_NAME}
            ${SOURCES}
    )
endif ()

target_compile_options(${PROJECT_NAME} PRIVATE /bigobj)

add_dependencies(${PROJECT_NAME} generate_bindings)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        sol2::sol2
        oatpp
        OGDF
        lua::lua54
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
        ${INCLUDE_DIR}/pch.hpp
)

message(STATUS "Engine project configuration completed successfully!")
message(STATUS " - Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS " - C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS " - Shared Library: ${BUILD_SHARED_LIBS}")
message(STATUS " - Generated Lua bindings: ${GENERATED_SRC}")