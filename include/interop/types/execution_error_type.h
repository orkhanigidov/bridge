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
