#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef enum ExecutionType
    {
        Lua_Script = 0,
        Lua_File = 1,
        Pipeline = 2,
    } ExecutionType;
} // namespace engine::interop::types

#pragma pack(pop)
