#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef enum ExecutionErrorType
    {
        Invalid_Argument = 0,
        Execution_Failed = 1,
        Validation_Failed = 2,
        File_Not_Found = 3
    } ExecutionErrorType;
} // namespace engine::interop::types

#pragma pack(pop)
