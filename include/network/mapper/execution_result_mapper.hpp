#pragma once

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
                dto->status = dto::execution::ExecutionStatusDto::SUCCESS;

                const auto output_dto = dto::execution::FileDto::createShared();
                output_dto->id = "output_graph";
                output_dto->chunk_index = static_cast<uint16_t>(0);
                output_dto->total_chunks = 1;
                output_dto->chunk_data = result.output_data;
                dto->output_data = output_dto;

                const auto metadata_dto = dto::execution::MetadataDto::createShared();
                metadata_dto->duration_milliseconds = result.metadata.duration_milliseconds;
                dto->metadata = metadata_dto;
            }
            else
            {
                dto->status = dto::execution::ExecutionStatusDto::FAILURE;

                const auto error_dto = dto::execution::ErrorDto::createShared();
                error_dto->type = ErrorMapper::to_dto(static_cast<interop::types::ExecutionErrorType>(result.error.type));
                error_dto->message = result.error.message;
                dto->error = error_dto;
            }

            return dto;
        }
    };
} // namespace engine::network::mapper
