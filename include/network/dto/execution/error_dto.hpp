/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file error_dto.hpp
 * @brief Defines the ErrorDto DTO for execution errors in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "error_type_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class ErrorDto
     * @brief Data transfer object for execution errors.
     *
     * Contains the error type and a descriptive error message.
     */
    class ErrorDto final : public oatpp::DTO
    {
        DTO_INIT(ErrorDto, DTO)

        /**
         * @brief The type of execution error.
         */
        DTO_FIELD(oatpp::Enum<ExecutionErrorTypeDto>::AsString, type);

        /**
         * @brief A descriptive error message.
         */
        DTO_FIELD(oatpp::String, message);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
