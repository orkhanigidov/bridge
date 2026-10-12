/**
 * Project: Engine
 * File: source_code_dto.hpp
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
 * @file source_code_dto.hpp
 * @brief Defines the SourceCodeDto DTO for transferring original source codes.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class SourceCodeDto
     * @brief Data transfer object for transferring C++ and Lua source codes.
     */
    class SourceCodeDto final : public oatpp::DTO
    {
        DTO_INIT(SourceCodeDto, DTO)

        /**
         * @brief C++ source code.
         */
        DTO_FIELD(oatpp::String, cpp_code);

        /**
         * @brief Lua source code.
         */
        DTO_FIELD(oatpp::String, lua_code);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
