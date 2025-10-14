#pragma once

#include <oatpp/core/Types.hpp>

#include "interop/types/execution_error_type.h"

namespace engine::execution
{
    struct ExecutionError
    {
        interop::types::ExecutionErrorType type;
        std::string message;
    };

    struct ExecutionResult
    {
        bool is_success;
        oatpp::String output_data;
        ExecutionError error;
    };
}
