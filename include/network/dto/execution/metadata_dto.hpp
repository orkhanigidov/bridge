/**
 * Project: Engine
 * File: metadata_dto.hpp
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
 * @file metadata_dto.hpp
 * @brief Defines the MetadataDto DTO for execution metadata in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class MetadataDto
     * @brief Data transfer object for execution metadata.
     *
     * Contains metadata such as the duration of execution in milliseconds.
     */
    class MetadataDto final : public oatpp::DTO
    {
        DTO_INIT(MetadataDto, DTO)

        /**
         * @brief Duration of execution in milliseconds.
         */
        DTO_FIELD(oatpp::UInt64, duration_milliseconds);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
