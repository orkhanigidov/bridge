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
        static ExecutionResponsePtr create_error(interop::types::ExecutionStatus status,
                                                 interop::types::ExecutionErrorType type, const char* message);
        static ExecutionResponsePtr create_success(uint64_t duration_milliseconds);
    };

} // namespace engine::utils