#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "execution/pipeline/pipeline_step.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto {

    class PipelineDto final : public oatpp::DTO {
        DTO_INIT(PipelineDto, DTO)

        DTO_FIELD(String, pipeline_task, "task");
        DTO_FIELD(List<Object<PipelineStepDto>>, pipeline_steps, "steps");
    };

} // namespace engine::network::dto

#include OATPP_CODEGEN_END(DTO)
