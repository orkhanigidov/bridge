#pragma once

#include <cstdint>

#pragma pack(push, 8)

namespace engine::interop::types
{
    typedef struct ExecutionOptions
    {
        uint32_t timeout_milliseconds;
        const char* output_data_format;
    } ExecutionOptions;
} // namespace engine::interop::types

#pragma pack(pop)
