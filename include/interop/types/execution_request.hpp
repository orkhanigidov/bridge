#pragma once

#include "execution_options.hpp"
#include "execution_type.hpp"

#pragma pack(push, 8)

namespace engine::interop::types {

    struct ExecutionRequest {
        ExecutionType type = ExecutionType::Unknown;
        const char* script_path{};
        ExecutionOptions options;
    };

} // namespace engine::interop::types

#pragma pack(pop)