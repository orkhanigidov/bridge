#pragma once

#if defined(_WIN32)
#if defined(ENGINE_BUILDING_DLL)
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#else
#define ENGINE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

    ENGINE_API bool ExecuteScript(const char* script_path,
                                  const char* input_data_path,
                                  const char* output_data_path);

#ifdef __cplusplus
} // extern "C"
#endif
