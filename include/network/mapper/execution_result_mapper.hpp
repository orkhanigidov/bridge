/**
 * Project: Engine
 * File: execution_result_mapper.hpp
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
 * @file execution_result_mapper.hpp
 * @brief Declares the ExecutionResultMapper utility for mapping core execution results to DTO responses.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <oatpp/core/Types.hpp>

#include "error_mapper.hpp"
#include "execution/core_execution_result.hpp"
#include "interop/types/execution_error_type.h"
#include "network/dto/execution/error_dto.hpp"
#include "network/dto/execution/file_dto.hpp"
#include "network/dto/execution/metadata_dto.hpp"
#include "network/dto/execution/response_dto.hpp"
#include "network/dto/execution/status_dto.hpp"

namespace engine::network::mapper
{
    /**
     * @class ExecutionResultMapper
     * @brief Utility class for mapping core execution results to DTO responses.
     */
    class ExecutionResultMapper final
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        ExecutionResultMapper() = delete;

        /**
         * @brief Converts a CoreExecutionResult to a ResponseDto object, chunking output data if necessary.
         * @param result The core execution result.
         * @return The corresponding ResponseDto object.
         */
        static oatpp::Object<dto::execution::ResponseDto> to_dto(const execution::CoreExecutionResult& result)
        {
            const auto dto = dto::execution::ResponseDto::createShared();

            if (!result.string_output.empty())
            {
                dto->string_output = result.string_output.c_str();
            }

            if (result.is_success())
            {
                dto->status = dto::execution::ExecutionStatusDto::Success;

                const auto& full_data = result.output_data;
                const size_t total_length = full_data.length();

                auto chunks = oatpp::List<oatpp::Object<dto::execution::FileDto>>::createShared();

                if (total_length <= MAX_CHUNK_SIZE)
                {
                    const auto output_dto = dto::execution::FileDto::createShared();
                    output_dto->id = "output_graph";
                    output_dto->chunk_index = static_cast<uint16_t>(0);
                    output_dto->total_chunks = 1;
                    output_dto->chunk_data = full_data;
                    chunks->emplace_back(output_dto);
                }
                else
                {
                    const auto total_chunks = static_cast<uint16_t>(std::ceil(static_cast<double>(total_length) / MAX_CHUNK_SIZE));

                    for (uint16_t i = 0; i < total_chunks; ++i)
                    {
                        size_t offset = i * MAX_CHUNK_SIZE;
                        size_t length = std::min(MAX_CHUNK_SIZE, total_length - offset);

                        auto chunk_data = oatpp::String(full_data.data() + offset, length);

                        const auto output_dto = dto::execution::FileDto::createShared();
                        output_dto->id = "output_graph";
                        output_dto->chunk_index = i;
                        output_dto->total_chunks = total_chunks;
                        output_dto->chunk_data = chunk_data;
                        chunks->emplace_back(output_dto);
                    }
                }

                dto->output_data = chunks;

                const auto metadata_dto = dto::execution::MetadataDto::createShared();
                metadata_dto->duration_milliseconds = result.metadata.duration_milliseconds;
                dto->metadata = metadata_dto;
            }
            else
            {
                dto->status = dto::execution::ExecutionStatusDto::Failure;

                const auto error_dto = dto::execution::ErrorDto::createShared();
                error_dto->type = ErrorMapper::to_dto(static_cast<interop::types::ExecutionErrorType>(result.error.type));
                error_dto->message = result.error.message;
                dto->error = error_dto;
            }

            return dto;
        }

    private:
        /**
         * @brief Maximum size of each output data chunk (1 MB).
         */
        static constexpr size_t MAX_CHUNK_SIZE = 1 * 1024 * 1024; // 1 MB
    };
} // namespace engine::network::mapper
