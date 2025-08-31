#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "file_dto.hpp"
#include "options_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace network::dto::execution {

    ENUM(ExecutionType, v_int8,
        VALUE(SCRIPT, 0, "script"),
        VALUE(PIPELINE, 1, "pipeline")
    )

    class RequestDto final : public oatpp::DTO {
        DTO_INIT(RequestDto, DTO)

        DTO_FIELD(Enum<ExecutionType>::AsString, type);
        DTO_FIELD(String, script);
        DTO_FIELD(Object<FileDto>, input_data);
        DTO_FIELD(Object<OptionsDto>, options);
    };

} // namespace network::dto::execution

#include OATPP_CODEGEN_END(DTO)
