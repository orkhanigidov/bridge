/**
 * Project: Engine
 * File: response_dto.hpp
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
 * @file sequential_dto.hpp
 * @brief Defines the SequentialRequestDto and SequentialResponseDto DTOs for execution requests and responses in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class SequentialRequestDto
     * @brief Data transfer object for sequential execution requests.
     *
     * Contains the C++ and Lua code to be executed, as well as the path to the input GraphML file.
     */
    class SequentialRequestDto final : public oatpp::DTO
    {
        DTO_INIT(SequentialRequestDto, DTO)

        DTO_FIELD(oatpp::String, cpp_code);
        DTO_FIELD(oatpp::String, lua_code);

        DTO_FIELD(oatpp::String, graphml_file);
    };

    /**
     * @class SequentialResponseDto
     * @brief Data transfer object for sequential execution responses.
     *
     * Contains the SVG outputs from the executed C++ and Lua scripts.
     */
    class SequentialResponseDto final : public oatpp::DTO
    {
        DTO_INIT(SequentialResponseDto, DTO)

        DTO_FIELD(oatpp::String, cpp_svg);
        DTO_FIELD(oatpp::String, lua_svg);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)