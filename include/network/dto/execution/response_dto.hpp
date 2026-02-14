/**
 * Project: Engine
 * File: response_dto.hpp
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
 * @file response_dto.hpp
 * @brief Defines the ResponseDto DTO for execution responses in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "error_dto.hpp"
#include "file_dto.hpp"
#include "metadata_dto.hpp"
#include "status_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class ResponseDto
     * @brief Data transfer object for execution responses.
     *
     * Contains the execution status, output data files, error information, and execution metadata.
     */
    class ResponseDto final : public oatpp::DTO
    {
        DTO_INIT(ResponseDto, DTO)

        /**
         * @brief The status of the execution.
         */
        DTO_FIELD(oatpp::Enum<ExecutionStatusDto>::AsString, status);

        /**
         * @brief List of output data files as file chunks.
         */
        DTO_FIELD(oatpp::List<oatpp::Object<FileDto>>, output_data);

        /**
         * @brief Optional string output from the script.
         */
        DTO_FIELD(oatpp::String, string_output);

        /**
         * @brief Error information, if any, occurred during execution.
         */
        DTO_FIELD(oatpp::Object<ErrorDto>, error);

        /**
         * @brief Metadata about the execution, such as duration.
         */
        DTO_FIELD(oatpp::Object<MetadataDto>, metadata);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
