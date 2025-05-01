include(CMakeParseArguments)

# Find Python interpreter
find_package(Python COMPONENTS Interpreter REQUIRED)
if (NOT Python_FOUND)
    message(FATAL_ERROR "Python interpreter not found. Please install Python 3.12+")
endif ()

# Define Python virtual environment paths
set(VENV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.venv)
if (WIN32)
    set(VENV_BIN ${VENV_DIR}/Scripts)
else ()
    set(VENV_BIN ${VENV_DIR}/bin)
endif ()

set(VENV_PYTHON ${VENV_BIN}/python${CMAKE_EXECUTABLE_SUFFIX})
set(VENV_PIP ${VENV_BIN}/pip${CMAKE_EXECUTABLE_SUFFIX})
set(VENV_CONAN ${VENV_BIN}/conan${CMAKE_EXECUTABLE_SUFFIX})
set(VENV_MARKER ${VENV_DIR}/.initialized)
set(CONAN_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps)
set(CONAN_MARKER ${CONAN_BUILD_DIR}/.conan_initialized)

# Setup Python virtual environment if it doesn't exist yet
function(setup_python_environment)
    add_custom_command(
            OUTPUT ${VENV_MARKER}
            COMMAND ${CMAKE_COMMAND} -E echo "Setting up Python virtual environment..."
            COMMAND ${CMAKE_COMMAND} -E make_directory ${VENV_DIR}
            COMMAND ${Python_EXECUTABLE} -m venv ${VENV_DIR}
            COMMAND ${CMAKE_COMMAND} -E echo "Installing required Python packages..."
            COMMAND ${VENV_PIP} install -U pip
            COMMAND ${VENV_PIP} install -r ${CMAKE_CURRENT_SOURCE_DIR}/scripts/codegen/requirements.txt
            COMMAND ${VENV_PIP} install -r ${CMAKE_CURRENT_SOURCE_DIR}/scripts/third_party/requirements.txt
            COMMAND ${CMAKE_COMMAND} -E touch ${VENV_MARKER}
            VERBATIM
    )

    add_custom_target(setup-python-environment DEPENDS ${VENV_MARKER})
endfunction()

function(setup_conan_dependencies)
    add_custom_command(
            OUTPUT ${CONAN_MARKER}
            DEPENDS setup-python-environment ${VENV_MARKER}
            COMMAND ${CMAKE_COMMAND} -E echo "Installing C++ dependencies with Conan..."
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CONAN_BUILD_DIR}
            COMMAND ${VENV_CONAN} install ${CMAKE_CURRENT_SOURCE_DIR} --output-folder=${CONAN_BUILD_DIR} --build=missing
            COMMAND ${CMAKE_COMMAND} -E touch ${CONAN_MARKER}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            VERBATIM
    )

    add_custom_target(setup-conan-dependencies DEPENDS ${CONAN_MARKER})
endfunction()

function(find_required_packages)
    include(${CONAN_BUILD_DIR}/conan_toolchain.cmake OPTIONAL RESULT_VARIABLE CONAN_TOOLCHAIN_FOUND)

    if (CONAN_TOOLCHAIN_FOUND)
        message(STATUS "Conan toolchain found and loaded")

        find_package(nlohmann_json REQUIRED)
        find_package(rttr REQUIRED)
        find_package(cppzmq REQUIRED)

        foreach (req_target nlohmann_json::nlohmann_json RTTR::Core_Lib cppzmq)
            if (NOT TARGET ${req_target})
                message(FATAL_ERROR "Required target ${req_target} not found. Conan dependencies may have failed to install properly.")
            endif ()
        endforeach ()
    else ()
        message(FATAL_ERROR "Conan toolchain not found. Run 'cmake --build . --target setup-conan-dependencies' first to install required dependencies.")
    endif ()
endfunction()

function(link_common_dependencies target)
    if (NOT TARGET ${target})
        message(FATAL_ERROR "Target '${target}' does not exist. Make sure the target is defined before calling link_common_dependencies().")
    endif ()

    add_dependencies(${target} setup-conan-dependencies)
    target_link_libraries(${target} PRIVATE nlohmann_json::nlohmann_json RTTR::Core_Lib cppzmq)
endfunction()

# Setup all dependencies
setup_python_environment()
setup_conan_dependencies()
find_required_packages()
