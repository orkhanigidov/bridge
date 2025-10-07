#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef struct ExecutionMetadata
    {
        uint64_t duration_milliseconds;
    } ExecutionMetadata;
} // namespace engine::interop::types

#pragma pack(pop)
