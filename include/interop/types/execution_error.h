#pragma once

#include "execution_error_type.h"

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef struct ExecutionError
    {
        ExecutionErrorType type;
        char* message;
    } ExecutionError;
} // namespace engine::interop::types

#pragma pack(pop)
