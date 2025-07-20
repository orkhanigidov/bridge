#pragma once

#include "engine/native/native_bridge.hpp"

namespace engine::native
{
    bool ExecuteScript(const char* script_path)
    {
        return NativeBridge::execute_script(script_path);
    }
} // namespace engine::native
