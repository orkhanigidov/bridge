#pragma once

#include "error_mapper.hpp"
#include "metadata_mapper.hpp"
#include "network/dto/execution/response_dto.hpp"

namespace engine::mapper
{
    using namespace engine::network::dto::execution;
    using namespace engine::interop::types;

    class RequestMapper final
    {
    public:
        static oatpp::Enum<ExecutionStatusDto> to_dto(const ExecutionStatus& status)
        {
            return static_cast<ExecutionStatusDto>(status);
        }

        static oatpp::Object<ResponseDto> to_dto(const ExecutionResponse& response, const oatpp::Object<FileDto>& output_data)
        {
            auto dto = ResponseDto::createShared();
            dto->status = to_dto(response.status);
            dto->output_data = output_data;
            dto->error = ErrorMapper::to_dto(response.error);
            dto->metadata = MetadataMapper::to_dto(response.metadata);
            return dto;
        }
    };
} // namespace engine::mapper
