/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
