#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto {

    class MessageDto final : public oatpp::DTO {
        DTO_INIT(MessageDto, DTO)

        DTO_FIELD(UInt16, status_code);
        DTO_FIELD(String, message);
    };

} // namespace engine::network::dto

#include OATPP_CODEGEN_END(DTO)
