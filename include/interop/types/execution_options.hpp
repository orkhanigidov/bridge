#pragma once

namespace interop::types {

    struct ExecutionOptions {
        uint32_t timeout_ms = 0; // 0 means no timeout
        const char* output_data_format;
    };

} // namespace interop::types
