/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file error_mapper.hpp
 * @brief Declares the ErrorMapper utility for mapping execution errors between internal and DTO representations.
 */

#pragma once

#include <oatpp/core/Types.hpp>

#include "interop/types/execution_error.h"
#include "interop/types/execution_error_type.h"
#include "network/dto/execution/error_dto.hpp"
#include "network/dto/execution/error_type_dto.hpp"

namespace engine::network::mapper
{
    /**
     * @class ErrorMapper
     * @brief Utility class for mapping execution errors between internal and DTO representations.
     */
    class ErrorMapper final
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        ErrorMapper() = delete;

        /**
         * @brief Converts internal ExecutionErrorType to a DTO enum.
         * @param type The internal execution error type.
         * @return The corresponding ExecutionErrorTypeDto enum.
         */
        static oatpp::Enum<dto::execution::ExecutionErrorTypeDto> to_dto(const interop::types::ExecutionErrorType& type)
        {
            return static_cast<dto::execution::ExecutionErrorTypeDto>(type);
        }

        /**
         * @brief Converts internal ExecutionError to an ErrorDto object.
         * @param error The internal execution error.
         * @return The corresponding ErrorDto object, or nullptr if the message is null.
         */
        static oatpp::Object<dto::execution::ErrorDto> to_dto(const interop::types::ExecutionError& error)
        {
            if (error.message == nullptr)
            {
                return nullptr;
            }

            auto dto = dto::execution::ErrorDto::createShared();
            dto->type = to_dto(error.type);
            dto->message = error.message;
            return dto;
        }
    };
} // namespace engine::network::mapper
