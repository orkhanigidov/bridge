#pragma once

namespace engine::interop::types {

    enum class ExecutionStatus : uint8_t {
        Unknown,
        Success,
        Failure,
        Timeout
    };

} // namespace engine::interop::types
