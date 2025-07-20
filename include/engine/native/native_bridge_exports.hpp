#pragma once

#if defined(_WIN32)
    #define DLL_EXPORT extern "C" __declspec(dllexport)
#else
    #define DLL_EXPORT extern "C"
#endif

namespace engine::native
{
    DLL_EXPORT bool ExecuteScript(const char* script_path);
} // namespace engine::native
