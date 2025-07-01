#pragma once

#include "PipelineStep.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    class Pipeline final : public oatpp::DTO
    {
        DTO_INIT(Pipeline, DTO)

        DTO_FIELD_INFO(pipeline_name)
        {
            info->description = "Pipeline name";
            info->required    = true;
        }
        DTO_FIELD(String, pipeline_name, "pipeline");

        DTO_FIELD_INFO(steps)
        {
            info->description = "Pipeline execution steps";
            info->required    = true;
        }
        DTO_FIELD(List<Object<PipelineStep>>, steps);
    };

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto