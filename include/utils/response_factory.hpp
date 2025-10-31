/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <memory>
#include <string>

#include "interop/types/execution_error_type.h"
#include "interop/types/execution_metadata.h"
#include "interop/types/execution_response.h"
#include "interop/types/execution_status.h"

namespace engine::utils
{
    struct ExecutionResponseDeleter
    {
        void operator()(interop::types::ExecutionResponse* ptr) const;
    };

    using ExecutionResponsePtr = std::unique_ptr<interop::types::ExecutionResponse, ExecutionResponseDeleter>;

    class ResponseFactory final
    {
    public:
        ResponseFactory() = delete;

        static ExecutionResponsePtr create_error(interop::types::ExecutionStatus status,
                                                 interop::types::ExecutionErrorType type,
                                                 const std::string& message);
        static ExecutionResponsePtr create_success(interop::types::ExecutionMetadata metadata);
    };
} // namespace engine::utils
