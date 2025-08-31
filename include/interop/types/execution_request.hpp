#pragma once

#include "execution_options.hpp"
#include "execution_type.hpp"

namespace interop::types {

    struct ExecutionRequest {
        ExecutionType type;
        const char* script_path;
        const char* input_data_path;
        ExecutionOptions options;
    };

} // namespace interop::types
