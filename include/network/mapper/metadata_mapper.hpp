#pragma once

#include "interop/types/execution_metadata.h"
#include "network/dto/execution/metadata_dto.hpp"

namespace engine::network::mapper
{
    class MetadataMapper final
    {
    public:
        MetadataMapper() = delete;

        static oatpp::Object<dto::execution::MetadataDto> to_dto(const interop::types::ExecutionMetadata& metadata)
        {
            auto dto = dto::execution::MetadataDto::createShared();
            dto->duration_milliseconds = metadata.duration_milliseconds;
            return dto;
        }

        static interop::types::ExecutionMetadata from_dto(const oatpp::Object<dto::execution::MetadataDto>& dto)
        {
            interop::types::ExecutionMetadata metadata{};
            if (dto)
            {
                metadata.duration_milliseconds = dto->duration_milliseconds;
            }
            return metadata;
        }
    };
} // namespace engine::network::mapper
