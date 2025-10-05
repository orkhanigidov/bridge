#pragma once

#pragma pack(push, 8)

namespace engine::interop::types {

    struct ExecutionMetadata {
        uint64_t duration_milliseconds;
    };

} // namespace engine::interop::types

#pragma pack(pop)