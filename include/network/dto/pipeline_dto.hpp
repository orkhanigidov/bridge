#pragma once

#include "pipeline_step_dto.hpp"
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dto {
    class PipelineDto final : public oatpp::DTO {
        DTO_INIT(PipelineDto, DTO)

        DTO_FIELD_INFO(id)
        {
            info->description = "Unique identifier for the pipeline";
            info->required = true;
        }
        DTO_FIELD(String, id);

        DTO_FIELD_INFO(steps)
        {
            info->description = "Ordered list of pipeline steps";
            info->required = true;
        }
        DTO_FIELD(List<Object<PipelineStepDto>>, steps);
    };
} // namespace dto

#include OATPP_CODEGEN_END(DTO)
