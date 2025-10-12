#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef enum ExecutionType
    {
        Lua_Script = 0,
        Pipeline = 1,
    } ExecutionType;
} // namespace engine::interop::types

#pragma pack(pop)
