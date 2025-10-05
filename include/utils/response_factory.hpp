#pragma once

#include "interop/interop_bridge.hpp"

namespace engine::utils
{

    namespace ResponseFactory
    {
        std::unique_ptr<interop::types::ExecutionResponse> create_error(interop::types::ExecutionStatus status, interop::types::ExecutionErrorType type, const char* message);
        std::unique_ptr<interop::types::ExecutionResponse> create_success(uint64_t duration_milliseconds);
    }

} // namespace engine::utils