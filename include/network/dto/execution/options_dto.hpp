#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace network::dto::execution {

    class OptionsDto final : public oatpp::DTO {
        DTO_INIT(OptionsDto, DTO)

        DTO_FIELD(UInt32, timeout_ms);
        DTO_FIELD(String, output_data_format);
    };

} // namespace network::dto::execution

#include OATPP_CODEGEN_END(DTO)
