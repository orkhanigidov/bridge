#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    class StepDTO final : public oatpp::DTO
    {
        DTO_INIT(StepDTO, DTO)

        DTO_FIELD_INFO(obj)
        {
            info->description = "Create new object instance";
        }
        DTO_FIELD(String, obj, "new");

        DTO_FIELD_INFO(alias)
        {
            info->description = "Alias for the created object";
        }
        DTO_FIELD(String, alias, "as");

        DTO_FIELD_INFO(func)
        {
            info->description = "Function to call";
        }
        DTO_FIELD(String, func, "call");

        DTO_FIELD_INFO(params)
        {
            info->description = "Parameters for the function and object creation";
        }
        DTO_FIELD(List<Any>, params, "with");
    };

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto