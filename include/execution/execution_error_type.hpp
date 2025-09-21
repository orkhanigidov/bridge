#pragma once

namespace execution {

    enum class ExecutionErrorType : uint8_t {
        Invalid_Argument,
        Invalid_Format,
        Invalid_Conversion,
        Execution_Failed,
        Conversion_Failed,
        Validation_Failed
    };

} // namespace execution
