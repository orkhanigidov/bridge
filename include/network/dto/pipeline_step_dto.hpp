#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dto {
    class PipelineStepDto final : public oatpp::DTO {
        DTO_INIT(PipelineStepDto, DTO)

        DTO_FIELD_INFO(class_name)
        {
            info->description = "Class name for object instantiation";
            info->required = false;
        }
        DTO_FIELD(String, class_name, "create");

        DTO_FIELD_INFO(alias)
        {
            info->description
                    = "Variable name reference for the instantiated object";
            info->required = false;
        }
        DTO_FIELD(String, alias, "as");

        DTO_FIELD_INFO(function_name)
        {
            info->description = "Function or method name to call";
            info->required = false;
        }
        DTO_FIELD(String, function_name, "call");

        DTO_FIELD_INFO(constructor_args)
        {
            info->description = "Arguments passed to constructors";
            info->required = false;
        }
        DTO_FIELD(List<Any>, constructor_args, "with");

        DTO_FIELD_INFO(function_args)
        {
            info->description = "Arguments passed to functions or methods";
            info->required = false;
        }
        DTO_FIELD(List<Any>, function_args, "using");
    };
} // namespace dto

#include OATPP_CODEGEN_END(DTO)
