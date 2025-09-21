#pragma once
#include "execution_error_type.hpp"

namespace interop::types {

    struct ExecutionError {
        ExecutionErrorType error_type;
        const char* message;
    };

} // namespace interop::types
