/**
 * Project: Engine
 * File: status_dto.hpp
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
 * @file status_dto.hpp
 * @brief Defines the ExecutionStatusDto enum DTO for execution status in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @enum ExecutionStatusDto
     * @brief Enumerates execution status values for network responses.
     *
     * - Success: Execution completed successfully.
     * - Failure: Execution failed.
     * - Timeout: Execution timed out.
     */
    ENUM(ExecutionStatusDto, v_int8,
         VALUE(Success, 0, "Success"),
         VALUE(Failure, 1, "Failure"),
         VALUE(Timeout, 2, "Timeout")
    )
} // namespace engine::network::dto::execution
