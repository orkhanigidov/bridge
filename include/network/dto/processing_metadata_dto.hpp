#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dto {
    class ProcessingMetadataDto final : public oatpp::DTO {
        DTO_INIT(ProcessingMetadataDto, DTO)

        DTO_FIELD_INFO(processing_time_ms)
        {
            info->description = "Total processing time in milliseconds";
            info->required = true;
        }
        DTO_FIELD(Int64, processing_time_ms);
    };
} // namespace dto

#include OATPP_CODEGEN_END(DTO)
