#pragma once

#include "execution_error_type.hpp"

#pragma pack(push, 8)

namespace engine::interop::types {

    struct ExecutionError {
        ExecutionErrorType type;
        char* message;
    };

} // namespace engine::interop::types

#pragma pack(pop)