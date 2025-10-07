#pragma once

#include "network/dto/execution/metadata_dto.hpp"

namespace engine::mapper
{
    using namespace engine::network::dto::execution;
    using namespace engine::interop::types;

    class MetadataMapper final
    {
    public:
        static oatpp::Object<MetadataDto> to_dto(const ExecutionMetadata& metadata)
        {
            auto dto = MetadataDto::createShared();
            dto->duration_milliseconds = metadata.duration_milliseconds;
            return dto;
        }

        static ExecutionMetadata from_dto(const oatpp::Object<MetadataDto>& dto)
        {
            ExecutionMetadata metadata;
            metadata.duration_milliseconds = dto->duration_milliseconds;
            return metadata;
        }
    };
} // namespace engine::mapper
