#pragma once

#include "pipeline_dto.hpp"
#include "processing_options_dto.hpp"
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dto {
    class ExecutionRequestDto final : public oatpp::DTO {
        DTO_INIT(ExecutionRequestDto, DTO)

        DTO_FIELD_INFO(type)
        {
            info->description = "Execution mode. Supported values: "
                                "'lua_script' or 'json_pipeline'";
            info->required = true;
        }
        DTO_FIELD(String, type);

        DTO_FIELD_INFO(script)
        {
            info->description = "Script to execute when 'type' is 'lua_script'";
            info->required = false;
        }
        DTO_FIELD(String, script);

        DTO_FIELD_INFO(pipeline)
        {
            info->description
                    = "Pipeline to execute when 'type' is 'json_pipeline'";
            info->required = false;
        }
        DTO_FIELD(List<Object<PipelineDto>>, pipeline);

        DTO_FIELD_INFO(options)
        {
            info->description = "Execution options";
            info->required = false;
        }
        DTO_FIELD(Object<ProcessingOptionsDto>, options);
    };
} // namespace dto

#include OATPP_CODEGEN_END(DTO)
