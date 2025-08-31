#pragma once

namespace interop::types {

    enum class ExecutionStatus : uint8_t {
        Success,
        Failure,
        Timeout,
        Unknown
    };

} // namespace interop::types
