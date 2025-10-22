#pragma once

#include "interop/types/execution_type.h"
#include "utils/response_factory.hpp"

namespace engine::execution
{
    class ExecutionEngine final
    {
    public:
        ExecutionEngine() = delete;

        static utils::ExecutionResponsePtr execute(interop::types::ExecutionType type, const std::string& script);
    };
} // namespace engine::execution
