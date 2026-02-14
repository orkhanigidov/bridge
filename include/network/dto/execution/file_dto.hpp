/**
 * Project: Engine
 * File: file_dto.hpp
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
 * @file file_dto.hpp
 * @brief Defines the FileDto DTO for file chunk transfer in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class FileDto
     * @brief Data transfer object for file chunk transfer.
     *
     * Contains information about a file chunk, including its identifier, index, total number of chunks, and chunk data.
     */
    class FileDto final : public oatpp::DTO
    {
        DTO_INIT(FileDto, DTO)

        /**
         * @brief Unique identifier for the file.
         */
        DTO_FIELD(oatpp::String, id);

        /**
         * @brief Index of the current chunk (starting from 0).
         */
        DTO_FIELD(oatpp::UInt16, chunk_index);

        /**
         * @brief Total number of chunks for the file.
         */
        DTO_FIELD(oatpp::UInt16, total_chunks);

        /**
         * @brief Data of the current chunk (encoded as a string).
         */
        DTO_FIELD(oatpp::String, chunk_data);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
