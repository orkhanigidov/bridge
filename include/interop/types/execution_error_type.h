#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef enum ExecutionErrorType
    {
        Invalid_Argument = 0,
        Invalid_Input = 1,
        Invalid_Format = 2,
        Invalid_Conversion = 3,
        Execution_Failed = 4,
        Conversion_Failed = 5,
        Validation_Failed = 6,
        File_Not_Found = 7
    } ExecutionErrorType;
} // namespace engine::interop::types

#pragma pack(pop)
