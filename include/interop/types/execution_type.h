/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef enum ExecutionType
    {
        LuaScript = 0,
        LuaFile = 1,
        Pipeline = 2,
    } ExecutionType;
} // namespace engine::interop::types

#pragma pack(pop)
