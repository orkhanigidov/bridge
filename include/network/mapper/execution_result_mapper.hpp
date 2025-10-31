/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
    class ExecutionResultMapper final
    {
    public:
        ExecutionResultMapper() = delete;

        static oatpp::Object<dto::execution::ResponseDto> to_dto(const execution::CoreExecutionResult& result)
        {
            const auto dto = dto::execution::ResponseDto::createShared();

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
        static constexpr size_t MAX_CHUNK_SIZE = 1 * 1024 * 1024; // 1 MB
    };
} // namespace engine::network::mapper
