#pragma once

#include "error_mapper.hpp"
#include "interop/types/execution_response.h"
#include "interop/types/execution_status.h"
#include "metadata_mapper.hpp"
#include "network/dto/execution/response_dto.hpp"

namespace engine::network::mapper
{
    class ResponseMapper final
    {
    public:
        ResponseMapper() = delete;

        static oatpp::Enum<dto::execution::ExecutionStatusDto> to_dto(const interop::types::ExecutionStatus& status)
        {
            return static_cast<dto::execution::ExecutionStatusDto>(status);
        }

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
