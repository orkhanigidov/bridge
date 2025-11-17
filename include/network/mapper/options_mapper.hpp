/**
 * Project: Engine
 * File: options_mapper.hpp
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
 * @file options_mapper.hpp
 * @brief Declares the OptionsMapper utility for mapping execution options between internal and DTO representations.
 */

#pragma once

#include <oatpp/core/Types.hpp>

#include "interop/types/execution_options.h"
#include "network/dto/execution/options_dto.hpp"

namespace engine::network::mapper
{
    /**
     * @class  OptionsMapper
     * @brief Utility class for mapping exception options between internal and DTO representations.
     */
    class OptionsMapper final
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        OptionsMapper() = delete;

        /**
         * @brief Converts internal ExecutionOptions to an OptionsDto object.
         * @param options The internal execution options.
         * @return The corresponding OptionsDto object.
         */
        static oatpp::Object<dto::execution::OptionsDto> to_dto(const interop::types::ExecutionOptions& options)
        {
            auto dto = dto::execution::OptionsDto::createShared();
            dto->timeout_milliseconds = options.timeout_milliseconds;
            dto->output_data_format = options.output_data_format;
            return dto;
        }

        /**
         * @brief Converts an OptionsDto object to internal ExecutionOptions.
         * @param dto The OptionsDto object.
         * @return The corresponding internal ExecutionOptions.
         */
        static interop::types::ExecutionOptions from_dto(const oatpp::Object<dto::execution::OptionsDto>& dto)
        {
            interop::types::ExecutionOptions options{};
            if (dto)
            {
                options.timeout_milliseconds = dto->timeout_milliseconds;
                options.output_data_format = dto->output_data_format->c_str();
            }
            return options;
        }
    };
} // namespace engine::network::mapper
