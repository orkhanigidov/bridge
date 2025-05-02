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

# Load environment variables from .env file
include(cmake/EnvLoader.cmake)
load_env_file(${CMAKE_CURRENT_SOURCE_DIR}/.env)

# Required variable check
foreach (required_var ENV_ROOT_DIR)
    if (NOT DEFINED ${required_var})
        message(FATAL_ERROR "${required_var} is not defined. Please set it in the .env file.")
    endif ()
endforeach ()

include(cmake/Dependencies.cmake)

set(HEADERS
        include/core/Engine.h
        include/core/MethodDescriptor.h
        include/core/Registration.h
        include/network/NetworkManager.h
)

set(SOURCES
        src/main.cpp
        src/core/Engine.cpp
        src/core/MethodDescriptor.cpp
        src/network/NetworkManager.cpp
)

add_executable(Engine ${HEADERS} ${SOURCES})

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

target_include_directories(Engine PRIVATE ${ENV_ROOT_DIR}/${ENV_INCLUDE_DIR})

# Add dependency targets and link with external libraries
add_dependencies(Engine setup-python-environment setup-conan-dependencies)
link_common_dependencies(Engine)

if (DEFINED ENV_LIBRARIES)
    string(REPLACE ";" " " libs ${ENV_LIBRARIES})
    separate_arguments(libs)
    foreach (lib ${libs})
        get_filename_component(lib_path ${ENV_ROOT_DIR}/${lib} ABSOLUTE)
        if (EXISTS ${lib_path})
            target_link_libraries(Engine PRIVATE ${lib_path})
        else ()
            message(WARNING "Library '${lib_path}' not found.")
        endif ()
    endforeach ()
endif ()

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