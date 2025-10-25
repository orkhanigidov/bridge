#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    class OptionsDto final : public oatpp::DTO
    {
        DTO_INIT(OptionsDto, DTO)

        DTO_FIELD(oatpp::UInt32, timeout_milliseconds);
        DTO_FIELD(oatpp::String, output_data_format);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
