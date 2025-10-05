#pragma once

#include "execution_error.hpp"
#include "execution_metadata.hpp"
#include "execution_status.hpp"

#pragma pack(push, 8)

namespace engine::interop::types {

    struct ExecutionResponse {
        ExecutionStatus status;
        ExecutionError error;
        ExecutionMetadata metadata;
    };

} // namespace engine::interop::types

#pragma pack(pop)