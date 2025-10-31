/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef enum ExecutionErrorType
    {
        InvalidArgument = 0,
        ExecutionFailed = 1,
        ValidationFailed = 2,
        FileNotFound = 3
    } ExecutionErrorType;
} // namespace engine::interop::types

#pragma pack(pop)
