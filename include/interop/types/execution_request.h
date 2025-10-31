/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include "execution_options.h"
#include "execution_type.h"

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef struct ExecutionRequest
    {
        ExecutionType type;
        const char* script;
        ExecutionOptions options;
    } ExecutionRequest;
} // namespace engine::interop::types

#pragma pack(pop)
