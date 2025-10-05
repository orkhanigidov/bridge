#pragma once

#include "execution/pipeline/pipeline_executor.hpp"
#include "interop/types/execution_type.h"
#include "utils/response_factory.hpp"

namespace engine::execution
{

    class ExecutionEngine final
    {
      public:
        static ExecutionEngine& instance()
        {
            static ExecutionEngine instance;
            return instance;
        }

        static utils::ExecutionResponsePtr execute_lua(interop::types::ExecutionType type,
                                                       const std::string& script_or_path);
        static utils::ExecutionResponsePtr execute_pipeline(interop::types::ExecutionType type,
                                                            const std::string& json_or_path);

      private:
        ExecutionEngine() = default;
    };

} // namespace engine::execution
