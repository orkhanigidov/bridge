#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dto {
    class ProcessingOptionsDto final : public oatpp::DTO {
        DTO_INIT(ProcessingOptionsDto, DTO)

        DTO_FIELD_INFO(timeout)
        {
            info->description = "Execution timeout in milliseconds";
            info->required = true;
        }
        DTO_FIELD(Int32, timeout);

        DTO_FIELD_INFO(output_format)
        {
            info->description = "";
            info->required = false;
        }
        DTO_FIELD(String, output_format);
    };
} // namespace dto

#include OATPP_CODEGEN_END(DTO)
