/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <cstdint>

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef struct ExecutionMetadata
    {
        uint64_t duration_milliseconds;
    } ExecutionMetadata;
} // namespace engine::interop::types

#pragma pack(pop)
