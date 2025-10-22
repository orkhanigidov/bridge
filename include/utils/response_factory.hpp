#pragma once

#include "interop/types/execution_response.h"

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
