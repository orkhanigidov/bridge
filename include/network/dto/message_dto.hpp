#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dto {
    class MessageDto final : public oatpp::DTO {
        DTO_INIT(MessageDto, DTO)

        DTO_FIELD_INFO(status_code)
        {
            info->description = "HTTP status code";
            info->required = true;
        }
        DTO_FIELD(Int32, status_code);

        DTO_FIELD_INFO(message)
        {
            info->description = "Message";
            info->required = true;
        }
        DTO_FIELD(String, message);
    };
} // namespace dto

#include OATPP_CODEGEN_END(DTO)
