if (WIN32 OR MINGW)
    find_program(MINGW_GCC x86_64-w64-mingw32-gcc)
    find_program(MINGW_GXX x86_64-w64-mingw32-g++)

    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_SYSTEM_PROCESSOR x86_64)

    set(CMAKE_C_COMPILER "${MINGW_GCC}")
    set(CMAKE_CXX_COMPILER "${MINGW_GXX}")

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include cstdint" CACHE STRING "Force include cstdint for MinGW build")
endif ()