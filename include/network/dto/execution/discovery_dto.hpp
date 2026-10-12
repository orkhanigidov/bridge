/**
 * Project: Engine
 * File: discovery_dto.hpp
 *
 * Copyright (C) 2026 Orkhan Igidov
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
 * @file discovery_dto.hpp
 * @brief Defines the DiscoveryDto DTO for listing available methods and files.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class DiscoveryDto
     * @brief Data transfer object for discovery of available layout methods and graphml files.
     */
    class DiscoveryDto final : public oatpp::DTO
    {
        DTO_INIT(DiscoveryDto, DTO)

        /**
         * @brief List of available layout method names.
         */
        DTO_FIELD(oatpp::Vector<oatpp::String>, methods);

        /**
         * @brief List of available .graphml file names.
         */
        DTO_FIELD(oatpp::Vector<oatpp::String>, files);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
