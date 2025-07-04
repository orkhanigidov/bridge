#pragma once

#include "pipeline.hpp"

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    class execution_request final : public oatpp::DTO
    {
        DTO_INIT(execution_request, DTO)

        DTO_FIELD_INFO(type)
        {
            info->description =
                "Type of execution to perform, e.g., 'lua script' or 'json pipeline'";
            info->required = true;
        }
        DTO_FIELD(String, type);

        DTO_FIELD_INFO(pipelines)
        {
            info->description = "Collection of pipeline definitions to process";
            info->required    = false;
        }
        DTO_FIELD(List<Object<pipeline>>, pipelines);

        DTO_FIELD_INFO(script_path)
        {
            info->description = "File path to the executable script (native mode)";
            info->required    = false;
        }
        DTO_FIELD(String, script_path);

        DTO_FIELD_INFO(script_content)
        {
            info->description = "Content of the script to execute (network mode)";
            info->required    = false;
        }
        DTO_FIELD(String, script_content);
    };

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto