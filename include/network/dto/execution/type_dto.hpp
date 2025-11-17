/**
 * Project: Engine
 * File: type_dto.hpp
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
 * @file type_dto.hpp
 * @brief Defines the ExecutionTypeDto enum DTO for execution types in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @enum ExecutionTypeDto
     * @brief Enumerates execution types for network requests.
     *
     * - LuaScript: Represents a Lua script execution.
     * - Pipeline: Represents a pipeline execution.
     */
    ENUM(ExecutionTypeDto, v_int8,
         VALUE(LuaScript, 0, "LuaScript"),
         VALUE(Pipeline, 1, "Pipeline")
    )
} // namespace engine::network::dto::execution
