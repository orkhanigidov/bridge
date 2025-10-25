#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "pipeline_step_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::pipeline
{
    class PipelineDto final : public oatpp::DTO
    {
        DTO_INIT(PipelineDto, DTO)

        DTO_FIELD(oatpp::String, pipeline_task, "task");
        DTO_FIELD(oatpp::List<oatpp::Object<PipelineStepDto>>, pipeline_steps, "steps");
    };
} // namespace engine::network::dto::pipeline

#include OATPP_CODEGEN_END(DTO)
