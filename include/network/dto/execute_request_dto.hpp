#pragma once

#include "processing_options_dto.hpp"
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dto {
    class ExecuteRequestDto final : public oatpp::DTO {
        DTO_INIT(ExecuteRequestDto, DTO)

        DTO_FIELD_INFO(script)
        {
            info->description = "Source code to execute";
            info->required = true;
        }
        DTO_FIELD(String, script);

        DTO_FIELD_INFO(options)
        {
            info->description = "Execution options";
            info->required = false;
        }
        DTO_FIELD(Object<ProcessingOptionsDto>, options);
    };
} // namespace dto

#include OATPP_CODEGEN_END(DTO)
