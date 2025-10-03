#pragma once

namespace engine::interop::types {

    enum class ExecutionStatus : uint8_t {
        Success,
        Failure,
        Timeout,
        Unknown
    };

} // namespace engine::interop::types
