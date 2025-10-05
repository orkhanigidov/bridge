#pragma once

namespace engine::interop::types {

    enum class ExecutionType : uint8_t {
        Unknown,
        Lua_Script,
        Lua_Script_File,
        Json_Pipeline
    };

} // namespace engine::interop::types
