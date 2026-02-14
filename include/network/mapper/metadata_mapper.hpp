/**
 * Project: Engine
 * File: metadata_mapper.hpp
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
 * @file metadata_mapper.hpp
 * @brief Declares the MetadataMapper utility for mapping execution metadata between internal and DTO representations.
 */

#pragma once

#include <oatpp/core/Types.hpp>

#include "interop/types/execution_metadata.h"
#include "network/dto/execution/metadata_dto.hpp"

namespace engine::network::mapper
{
    /**
     * @class MetadataMapper
     * @brief Utility class for mapping execution metadata between internal and DTO representations.
     */
    class MetadataMapper final
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        MetadataMapper() = delete;

        /**
         * @brief Converts internal ExecutionMetadata to a MetadataDto object.
         * @param metadata The internal execution metadata.
         * @return The corresponding MetadataDto object.
         */
        static oatpp::Object<dto::execution::MetadataDto> to_dto(const interop::types::ExecutionMetadata& metadata)
        {
            auto dto = dto::execution::MetadataDto::createShared();
            dto->duration_milliseconds = metadata.duration_milliseconds;
            return dto;
        }

        /**
         * @brief Converts a MetadataDto object to internal ExecturionMetada.
         * @param dto The MetadataDto object.
         * @return The corresponding internal ExecutionMetadata.
         */
        static interop::types::ExecutionMetadata from_dto(const oatpp::Object<dto::execution::MetadataDto>& dto)
        {
            interop::types::ExecutionMetadata metadata{};
            if (dto)
            {
                metadata.duration_milliseconds = dto->duration_milliseconds;
            }
            return metadata;
        }
    };
} // namespace engine::network::mapper
