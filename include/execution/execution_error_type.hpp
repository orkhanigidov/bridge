#pragma once

namespace execution {
    enum class ExecutionErrorType : uint8_t {
        Invalid_Argument,
        Invalid_Format,
        Invalid_Conversion,
        Failed_Execution,
        Failed_Conversion,
        Failed_Validation
    };
} // namespace execution
