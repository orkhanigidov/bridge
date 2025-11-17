/**
 * Project: Engine
 * File: options_dto.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file options_dto.hpp
 * @brief Defines the OptionsDto DTO for execution options in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class OptionsDto
     * @brief Data transfer object for execution options.
     *
     * Contains options such as timeout and output data format for execution requests.
     */
    class OptionsDto final : public oatpp::DTO
    {
        DTO_INIT(OptionsDto, DTO)

        /**
         * @brief Timeout for execution in milliseconds.
         */
        DTO_FIELD(oatpp::UInt32, timeout_milliseconds);

        /**
         * @brief Desired output data format (e.g., "graphml").
         */
        DTO_FIELD(oatpp::String, output_data_format);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
