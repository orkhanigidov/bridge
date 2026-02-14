/**
 * Project: Engine
 * File: message_dto.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file message_dto.hpp
 * @brief Defines the MessageDto DTO for generic status messages in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto
{
    /**
     * @class MessageDto
     * @brief Data transfer object for generic status message.
     *
     * Contains a status code and a descriptive message.
     */
    class MessageDto final : public oatpp::DTO
    {
        DTO_INIT(MessageDto, DTO)

        /**
         * @brief Status code representing the result or state.
         */
        DTO_FIELD(oatpp::UInt16, status_code);

        /**
         * @brief Descriptive message.
         */
        DTO_FIELD(oatpp::String, message);
    };
} // namespace engine::network::dto

#include OATPP_CODEGEN_END(DTO)
