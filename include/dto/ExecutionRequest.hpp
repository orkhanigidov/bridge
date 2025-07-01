#pragma once

#include "ExecutionMode.hpp"
#include "Pipeline.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    class ExecutionRequest final : public oatpp::DTO
    {
        DTO_INIT(ExecutionRequest, DTO)

        DTO_FIELD_INFO(execution_mode)
        {
            info->description = "Mode for pipeline execution";
            info->required    = true;
        }
        DTO_FIELD(Enum<ExecutionMode>::AsString, execution_mode);

        DTO_FIELD_INFO(pipelines)
        {
            info->description = "List of pipelines to execute";
            info->required    = false;
        }
        DTO_FIELD(List<Object<Pipeline>>, pipelines);

        DTO_FIELD_INFO(script_path)
        {
            info->description = "Path to execution script";
            info->required    = false;
        }
        DTO_FIELD(String, script_path);
    };

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto