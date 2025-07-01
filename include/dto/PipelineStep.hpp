#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    class PipelineStep final : public oatpp::DTO
    {
        DTO_INIT(PipelineStep, DTO)

        DTO_FIELD_INFO(object_type)
        {
            info->description = "Type of object to create";
        }
        DTO_FIELD(String, object_type, "new");

        DTO_FIELD_INFO(alias)
        {
            info->description = "Alias for the created object";
        }
        DTO_FIELD(String, alias, "as");

        DTO_FIELD_INFO(function_name)
        {
            info->description = "Function to call";
        }
        DTO_FIELD(String, function_name, "call");

        DTO_FIELD_INFO(parameters)
        {
            info->description = "Parameters for function calls and object creation";
        }
        DTO_FIELD(List<Any>, parameters, "with");
    };

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto