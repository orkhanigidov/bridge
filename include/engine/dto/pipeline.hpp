#pragma once

#include "pipeline_step.hpp"

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    class pipeline final : public oatpp::DTO
    {
        DTO_INIT(pipeline, DTO)

        DTO_FIELD_INFO(id)
        {
            info->description = "Unique identifier for the pipeline";
            info->required    = true;
        }
        DTO_FIELD(String, id);

        DTO_FIELD_INFO(steps)
        {
            info->description = "Ordered sequence of steps to execute";
            info->required    = true;
        }
        DTO_FIELD(List<pipeline_step>, steps);
    };

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto