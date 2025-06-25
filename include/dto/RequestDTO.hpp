#pragma once

#include "PipelineDTO.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    class RequestDTO final : public oatpp::DTO
    {
        DTO_INIT(RequestDTO, DTO)

        DTO_FIELD_INFO(pipelines)
        {
            info->description = "List of pipelines to execute";
            info->required    = true;
        }
        DTO_FIELD(List<Object<PipelineDTO>>, pipelines);
    };

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto