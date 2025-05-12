cmake_minimum_required(VERSION 3.28)
project(json_serialization LANGUAGES CXX)

include(utility.cmake)

message(STATUS "Scanning " ${PROJECT_NAME} " module.")
message(STATUS "===========================")

loadFolder("files" HPP_FILES SRC_FILES)

add_library(${PROJECT_NAME} ${HPP_FILES} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:.>
)
target_link_libraries(${PROJECT_NAME} PRIVATE rapidjson RTTR::Core_Lib)

set_compiler_warnings(${PROJECT_NAME})

message(STATUS "Scanning " ${PROJECT_NAME} " module finished!")
message(STATUS "")