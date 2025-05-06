cmake_minimum_required(VERSION 3.28)
project(Engine VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard for all targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define project structure
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_GENERATED_DIR ${PROJECT_SOURCE_DIR}/generated)

# Create directory for generated source files
file(MAKE_DIRECTORY ${PROJECT_GENERATED_DIR})

include(cmake/Dependencies.cmake)

set(HEADERS
        include/core/reflection/MethodRegistry.h
        include/core/reflection/RegistrationMacros.h
        include/core/Engine.h
        include/model/MethodDescriptor.h
        include/network/NetworkManager.h
        include/serialization/JsonRttrConverter.h
        include/serialization/MethodJsonSerializer.h
)

set(SOURCES
        src/main.cpp
        src/core/reflection/MethodRegistry.cpp
        src/core/Engine.cpp
        src/network/NetworkManager.cpp
        src/serialization/JsonRttrConverter.cpp
        src/serialization/MethodJsonSerializer.cpp
)

add_executable(Engine ${HEADERS} ${SOURCES})

# Add precompiled header configuration
target_precompile_headers(Engine PRIVATE ${PROJECT_INCLUDE_DIR}/pch.h)

# Registry code generation
# This command runs generate_registry.py to create the implementation file whenever the script or configuration file changes
set(REFLECTION_GENERATED_FILE ${PROJECT_GENERATED_DIR}/ReflectionRegistration.cpp)
add_custom_command(
        OUTPUT ${REFLECTION_GENERATED_FILE}
        COMMAND ${CMAKE_COMMAND} -E echo "Generating registry code..."
        COMMAND ${VENV_PYTHON} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/codegen/generate_registry.py
        --config=${CMAKE_CURRENT_SOURCE_DIR}/config/reflection_config.yaml
        --templates=${CMAKE_CURRENT_SOURCE_DIR}/templates
        > ${REFLECTION_GENERATED_FILE}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/codegen/generate_registry.py ${CMAKE_CURRENT_SOURCE_DIR}/config/reflection_config.yaml
        VERBATIM
)

# Add the generated implementation to the build
target_sources(Engine PRIVATE ${REFLECTION_GENERATED_FILE})

# Add dependency targets and link with external libraries
add_dependencies(Engine setup-python-environment)
link_common_dependencies(Engine)

# Set compiler warnings
if (MSVC)
    target_compile_options(Engine PRIVATE /W4)
else ()
    target_compile_options(Engine PRIVATE -Wall -Wextra -Wpedantic)
endif ()

install(TARGETS Engine
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
)