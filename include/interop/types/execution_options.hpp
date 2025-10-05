#pragma once

#pragma pack(push, 8)

namespace engine::interop::types {

    struct ExecutionOptions {
        uint32_t timeout_milliseconds = 0; // 0 means no timeout
        const char* output_data_format{};
    };

} // namespace engine::interop::types

#pragma pack(pop)