/**
 * Project: Engine
 * File: error_type_dto.hpp
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
 * @file error_type_dto.hpp
 * @brief Defines the ExecutionErrorTypeDto enum DTO for execution error types in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @brief Enumerates execution error types for network responses.
     *
     * - InvalidArgument: The provided argument is invalid.
     * - ExecutionFailed: The execution process failed.
     * - ValidationFailed: Validation of input or output failed.
     * - FileNotFound: The required file was not found.
     */
    ENUM(ExecutionErrorTypeDto, v_int8,
         VALUE(InvalidArgument, 0, "InvalidArgument"),
         VALUE(ExecutionFailed, 1, "ExecutionFailed"),
         VALUE(ValidationFailed, 2, "ValidationFailed"),
         VALUE(FileNotFound, 3, "FileNotFound")
    )
} // namespace engine::network::dto::execution
