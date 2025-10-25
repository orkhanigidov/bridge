#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::pipeline
{
    class PipelineStepDto final : public oatpp::DTO
    {
        DTO_INIT(PipelineStepDto, DTO)

        DTO_FIELD(oatpp::String, class_name, "create");
        DTO_FIELD(oatpp::String, instance_alias, "as");
        DTO_FIELD(oatpp::List<oatpp::Any>, constructor_args, "with");
        DTO_FIELD(oatpp::String, function_name, "call");
        DTO_FIELD(oatpp::List<oatpp::Any>, function_args, "using");
    };
} // namespace engine::network::dto::pipeline

#include OATPP_CODEGEN_END(DTO)
