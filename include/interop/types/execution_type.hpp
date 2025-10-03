#pragma once

namespace engine::interop::types {

    enum class ExecutionType : uint8_t {
        Lua_Script,
        Json_Pipeline,
        Unknown
    };

} // namespace engine::interop::types
