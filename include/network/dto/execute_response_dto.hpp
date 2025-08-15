#pragma once

#include "processing_metadata_dto.hpp"
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dto {
    class ExecuteResponseDto final : public oatpp::DTO {
        DTO_INIT(ExecuteResponseDto, DTO)

        DTO_FIELD_INFO(status)
        {
            info->description = "Execution status";
            info->required = true;
        }
        DTO_FIELD(String, status);

        DTO_FIELD_INFO(output)
        {
            info->description = "Standard output produced by the execution";
            info->required = true;
        }
        DTO_FIELD(String, output);

        DTO_FIELD_INFO(error)
        {
            info->description = "Error message if execution failed";
            info->required = false;
        }
        DTO_FIELD(List<String>, error);

        DTO_FIELD_INFO(metadata)
        {
            info->description = "Additional execution metadata";
            info->required = true;
        }
        DTO_FIELD(Object<ProcessingMetadataDto>, metadata);
    };
} // namespace dto

#include OATPP_CODEGEN_END(DTO)
