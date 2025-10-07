#pragma once

#include "execution_error.h"
#include "execution_metadata.h"
#include "execution_status.h"

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef struct ExecutionResponse
    {
        ExecutionStatus status;
        ExecutionError error;
        ExecutionMetadata metadata;
    } ExecutionResponse;
} // namespace engine::interop::types

#pragma pack(pop)
