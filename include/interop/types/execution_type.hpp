#pragma once

namespace engine::interop::types {

    enum class ExecutionType : uint8_t {
        Unknown,
        Lua_Script,
        Json_Pipeline
    };

} // namespace engine::interop::types
