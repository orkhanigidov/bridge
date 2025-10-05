#pragma once

#include "execution_options.h"
#include "execution_type.h"

#pragma pack(push, 8)

namespace engine::interop::types
{

    typedef struct ExecutionRequest
    {
        ExecutionType type;
        const char* script_or_path;
        ExecutionOptions options;
    } ExecutionRequest;

} // namespace engine::interop::types

#pragma pack(pop)