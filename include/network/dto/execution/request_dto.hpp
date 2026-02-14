/**
 * Project: Engine
 * File: request_dto.hpp
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
 * @file request_dto.hpp
 * @brief Defines the RequestDto DTO for execution requests in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "file_dto.hpp"
#include "options_dto.hpp"
#include "type_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class RequestDto
     * @brief Data transfer object for execution requests.
     *
     * Contains the execution type, script, input data files, and execution options.
     */
    class RequestDto final : public oatpp::DTO
    {
        DTO_INIT(RequestDto, DTO)

        /**
         * @brief The type of execution to perform.
         */
        DTO_FIELD(oatpp::Enum<ExecutionTypeDto>::AsString, type);

        /**
         * @brief The script to the executed.
         */
        DTO_FIELD(oatpp::String, script);

        /**
         * @brief List of input data files as file chunks.
         */
        DTO_FIELD(oatpp::List<oatpp::Object<FileDto>>, input_data);

        /**
         * @brief Execution options such as timeout and output format.
         */
        DTO_FIELD(oatpp::Object<OptionsDto>, options);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
