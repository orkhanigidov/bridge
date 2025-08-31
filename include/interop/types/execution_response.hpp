#pragma once

#include "execution_error.hpp"
#include "execution_metadata.hpp"
#include "execution_status.hpp"

namespace interop::types {

    struct ExecutionResponse {
        ExecutionStatus status;
        const char* output_data_path;
        ExecutionError error;
        ExecutionMetadata metadata;

        ExecutionResponse(
            ExecutionStatus status_ = ExecutionStatus::Unknown,
            const char* output_data_path_ = nullptr,
            const ExecutionError& error_ = {},
            const ExecutionMetadata& metadata_ = {}):
        status(status_),
        output_data_path(output_data_path_),
        error(error_),
        metadata(metadata_) {}
    };

} // namespace interop::types
