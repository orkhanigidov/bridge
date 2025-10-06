#pragma once

#include "execution/pipeline/pipeline_executor.hpp"
#include "interop/types/execution_type.h"
#include "utils/response_factory.hpp"

namespace engine::execution
{

    class ExecutionEngine final
    {
      public:
        ExecutionEngine() = default;

        utils::ExecutionResponsePtr execute_lua(interop::types::ExecutionType type,
                                                       const std::string& script_or_path);
        utils::ExecutionResponsePtr execute_pipeline(interop::types::ExecutionType type,
                                                            const std::string& json_or_path);
    };

} // namespace engine::execution
