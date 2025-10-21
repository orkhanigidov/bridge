#pragma once

#include "interop/types/execution_error_type.h"

#include <oatpp/core/Types.hpp>

namespace engine::execution
{
    /**
     * @brief
     */
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
