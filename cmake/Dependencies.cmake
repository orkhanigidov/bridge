include(FetchContent)

FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.12.0)
FetchContent_Declare(rttr GIT_REPOSITORY https://github.com/rttrorg/rttr.git GIT_TAG v0.9.6)
FetchContent_Declare(libzmq GIT_REPOSITORY https://github.com/zeromq/libzmq.git GIT_TAG v4.3.5)
FetchContent_Declare(cppzmq GIT_REPOSITORY https://github.com/zeromq/cppzmq.git GIT_TAG v4.10.0)
FetchContent_Declare(ogdf GIT_REPOSITORY https://github.com/ogdf/ogdf.git GIT_TAG elderberry-202309)

set(BUILD_UNIT_TESTS OFF CACHE BOOL "Disable unit tests build" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable examples build" FORCE)
set(BUILD_DOCUMENTATION OFF CACHE BOOL "Disable documentation build" FORCE)
set(CUSTOM_DOXYGEN_STYLE OFF CACHE BOOL "Disable custom Doxygen style" FORCE)

set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "Disable tests build" FORCE)

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
set(VENV_MARKER ${VENV_DIR}/.initialized)

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
            COMMAND ${CMAKE_COMMAND} -E touch ${VENV_MARKER}
            VERBATIM
    )

    add_custom_target(setup-python-environment DEPENDS ${VENV_MARKER})
endfunction()

function(make_dependencies_available)
    FetchContent_MakeAvailable(json)
    FetchContent_MakeAvailable(rttr)
    FetchContent_MakeAvailable(libzmq)
    FetchContent_MakeAvailable(cppzmq)
    FetchContent_MakeAvailable(ogdf)
endfunction()

function(link_common_dependencies target)
    if (NOT TARGET ${target})
        message(FATAL_ERROR "Target '${target}' does not exist. Please check the target name.")
    endif ()

    target_link_libraries(${target} PRIVATE
            nlohmann_json::nlohmann_json
            RTTR::Core
            libzmq
            cppzmq
            OGDF COIN
    )
endfunction()

# Setup all dependencies
setup_python_environment()
make_dependencies_available()