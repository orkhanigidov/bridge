#pragma once

#if defined(_WIN32)
    #if defined(ENGINE_BUILDING_DLL)
        #define ENGINE_API __declspec(dllexport)
    #else
        #define ENGINE_API __declspec(dllimport)
    #endif
#else
    #define ENGINE_API __attribute__((visibility("default")))
#endif
