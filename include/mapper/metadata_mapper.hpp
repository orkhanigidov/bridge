#pragma once

#include "interop/types/execution_metadata.h"
#include "network/dto/execution/metadata_dto.hpp"

namespace engine::mapper
{
    class MetadataMapper final
    {
    public:
        static oatpp::Object<network::dto::execution::MetadataDto>to_dto(const interop::types::ExecutionMetadata& metadata)
        {
            auto dto = network::dto::execution::MetadataDto::createShared();
            dto->duration_milliseconds = metadata.duration_milliseconds;
            return dto;
        }

        static interop::types::ExecutionMetadata from_dto(const oatpp::Object<network::dto::execution::MetadataDto>& dto)
        {
            interop::types::ExecutionMetadata metadata{};
            if (dto)
            {
                metadata.duration_milliseconds = dto->duration_milliseconds;
            }
            return metadata;
        }
    };
} // namespace engine::mapper
