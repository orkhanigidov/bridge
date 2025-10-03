#pragma once

namespace engine::interop::types {

    enum class ExecutionErrorType : uint8_t {
        Invalid_Argument,
        Invalid_Input,
        Invalid_Format,
        Invalid_Conversion,
        Execution_Failed,
        Conversion_Failed,
        Validation_Failed,
        File_Not_Found
    };

} // namespace engine::interop::types
