#pragma once

#include "execution_options.hpp"
#include "execution_type.hpp"

namespace interop::types {

    struct ExecutionRequest {
        ExecutionType type;
        const char* script_path;
        const char* input_data_path;
        ExecutionOptions options;

        ExecutionRequest(
            ExecutionType type_ = ExecutionType::Unknown,
            const char* script_path_ = nullptr,
            const char* input_data_path_ = nullptr,
            const ExecutionOptions& options_ = {}):
        type(type_),
        script_path(script_path_),
        input_data_path(input_data_path_),
        options(options_) {}
    };

} // namespace interop::types
