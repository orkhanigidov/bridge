#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef enum ExecutionType
    {
        Lua_Script = 0,
        Lua_Script_File = 1,
        Json_Pipeline = 2,
        Json_Pipeline_File = 3
    } ExecutionType;
} // namespace engine::interop::types

#pragma pack(pop)
