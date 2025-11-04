cmake_minimum_required(VERSION 3.28)

if (NOT DEFINED CMAKE_POLICY_VERSION_MINIMUM)
    set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "Minimum required CMake policy version")
endif ()

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

if (WIN32)
    message(STATUS "Windows platform detected. Using Lua from third_party directory.")

    set(LUA_INCLUDE_DIR ${THIRD_PARTY_DIR}/lua/include)
    set(LUA_LIB ${THIRD_PARTY_DIR}/lua/lib/liblua54.a)
    set(LUA_BIN ${THIRD_PARTY_DIR}/lua/bin/lua54.dll)

    add_library(Lua::Lua UNKNOWN IMPORTED)
    set_target_properties(Lua::Lua PROPERTIES
            IMPORTED_LOCATION ${LUA_LIB}
            INTERFACE_INCLUDE_DIRECTORIES ${LUA_INCLUDE_DIR}
    )
else ()
    message(STATUS "Linux platform detected. Searching for Lua 5.4 on the system.")

    find_package(Lua)

    if (Lua_FOUND AND NOT TARGET Lua::Lua)
        add_library(Lua::Lua INTERFACE IMPORTED)
        set_target_properties(Lua::Lua PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
                INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}"
        )
    endif ()
endif ()

include(cmake/CPM.cmake)

CPMAddPackage("gh:jbeder/yaml-cpp#0.8.0")
CPMAddPackage("gh:ThePhD/sol2#v3.5.0")
CPMAddPackage("gh:oatpp/oatpp#1.3.1")
CPMAddPackage("gh:ogdf/ogdf#elderberry-202309")

add_subdirectory(tools/codegen)

set(SOURCES
        ${SRC_DIR}/bindings/lua/lua_binder.cpp
        ${SRC_DIR}/execution/script/lua_script_runner.cpp
        ${SRC_DIR}/execution/script/lua_state_manager.cpp
        ${SRC_DIR}/execution/script/script_executor.cpp
        ${SRC_DIR}/execution/execution_engine.cpp
        ${SRC_DIR}/execution/execution_service.cpp
        ${SRC_DIR}/interop/interop_bridge.cpp
        ${SRC_DIR}/interop/interop_c_api.cpp
        ${SRC_DIR}/network/serialization/oatpp_type_adapter.cpp
        ${SRC_DIR}/network/server/server_config.cpp
        ${SRC_DIR}/network/server/server_manager.cpp
        ${SRC_DIR}/utils/response_factory.cpp
        ${SRC_DIR}/main.cpp
        ${GENERATED_SRC}
)

add_custom_command(
        OUTPUT ${GENERATED_SRC}
        COMMAND $<TARGET_FILE:Codegen> "${PROJECT_SOURCE_DIR}/.env"
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

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LUA_BIN} $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying lua54.dll to build directory"
    )
endif ()

if (BUILD_SHARED_LIB AND WIN32)
    file(READ "${PROJECT_SOURCE_DIR}/.env" ENV_CONTENT)
    string(REGEX MATCH "UNITY_PROJECT_PATH=(.*)" _ ${ENV_CONTENT})
    if (CMAKE_MATCH_1)
        set(ENV{UNITY_PROJECT_PATH} ${CMAKE_MATCH_1})
    endif ()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> $ENV{UNITY_PROJECT_PATH}/Assets/Plugins/x86_64/
            COMMENT "Copying DLL to Unity Plugins directory"
    )
endif ()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /bigobj)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wa,-mbig-obj")
endif ()

add_dependencies(${PROJECT_NAME} generate_bindings)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        sol2::sol2
        oatpp
        Lua::Lua
        OGDF
)

message(STATUS "Engine project configuration completed successfully!")
message(STATUS " - Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS " - C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS " - Shared Library: ${BUILD_SHARED_LIBS}")
message(STATUS " - Generated Lua bindings: ${GENERATED_SRC}")