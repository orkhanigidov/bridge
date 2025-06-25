#pragma once

#include "StepDTO.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    class PipelineDTO final : public oatpp::DTO
    {
        DTO_INIT(PipelineDTO, DTO)

        DTO_FIELD_INFO(name)
        {
            info->description = "Pipeline name";
            info->required    = true;
        }
        DTO_FIELD(String, name, "pipeline");

        DTO_FIELD_INFO(steps)
        {
            info->description = "Pipeline execution steps";
            info->required    = true;
        }
        DTO_FIELD(List<Object<StepDTO>>, steps);
    };

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto