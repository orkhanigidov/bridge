/**
 * Project: Engine
 * File: response_mapper.hpp
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
 * @file response_mapper.hpp
 * @brief Declares the ResponseMapper utility for mapping execution responses between internal and DTO representations.
 */

#pragma once

#include <oatpp/core/Types.hpp>

#include "error_mapper.hpp"
#include "metadata_mapper.hpp"
#include "interop/types/execution_response.h"
#include "interop/types/execution_status.h"
#include "network/dto/execution/file_dto.hpp"
#include "network/dto/execution/response_dto.hpp"
#include "network/dto/execution/status_dto.hpp"

namespace engine::network::mapper
{
    /**
     * @class ResponseMapper
     * @brief Utility class for mapping execution responses between internal and DTO representations.
     */
    class ResponseMapper final
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        ResponseMapper() = delete;

        /**
         * @brief Converts internal ExecutionStatus to a DTO enum.
         * @param status The internal execution status.
         * @return The corresponding ExecutionStatusDto enum.
         */
        static oatpp::Enum<dto::execution::ExecutionStatusDto> to_dto(const interop::types::ExecutionStatus& status)
        {
            return static_cast<dto::execution::ExecutionStatusDto>(status);
        }

        /**
         * @brief Converts internal ExecutionResponse and output data to a ResponseDto object.
         * @param response The internal execution response.
         * @param output_data The output file data as a FileDto object.
         * @return The corresponding ResponseDto object.
         */
        static oatpp::Object<dto::execution::ResponseDto> to_dto(const interop::types::ExecutionResponse& response,
                                                                 const oatpp::Object<dto::execution::FileDto>& output_data)
        {
            auto dto = dto::execution::ResponseDto::createShared();
            dto->status = to_dto(response.status);
            dto->output_data = output_data;
            dto->error = ErrorMapper::to_dto(response.error);
            dto->metadata = MetadataMapper::to_dto(response.metadata);
            return dto;
        }
    };
} // namespace engine::network::mapper
