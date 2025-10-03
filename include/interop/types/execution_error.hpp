#pragma once
#include "execution_error_type.hpp"

namespace engine::interop::types {

    struct ExecutionError {
        ExecutionErrorType error_type;
        const char* message;
    };

} // namespace engine::interop::types
