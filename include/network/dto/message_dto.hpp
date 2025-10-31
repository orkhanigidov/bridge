/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto
{
    class MessageDto final : public oatpp::DTO
    {
        DTO_INIT(MessageDto, DTO)

        DTO_FIELD(oatpp::UInt16, status_code);
        DTO_FIELD(oatpp::String, message);
    };
} // namespace engine::network::dto

#include OATPP_CODEGEN_END(DTO)
