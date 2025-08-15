#pragma once

#include "interop/interop_bridge_exports.hpp"
#include "interop/interop_bridge.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    ENGINE_API bool ExecuteScript(const char* script_path,
                                  const char* input_data_path,
                                  const char* output_data_path)
    {
        if (!script_path || !input_data_path || !output_data_path) {
            return false;
        }

        return interop::execute_script(script_path, input_data_path,
                                       output_data_path);
    }

#ifdef __cplusplus
} // extern "C"
#endif
