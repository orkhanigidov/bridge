#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "file_dto.hpp"
#include "options_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    ENUM(ExecutionTypeDto, v_int8,
         VALUE(LUA_SCRIPT, 0, "lua_script"),
         VALUE(JSON_PIPELINE, 1, "json_pipeline")
    )

    class RequestDto final : public oatpp::DTO
    {
        DTO_INIT(RequestDto, DTO)

        DTO_FIELD(Enum<ExecutionTypeDto>::AsString, type);
        DTO_FIELD(String, script);
        DTO_FIELD(Object<FileDto>, input_data);
        DTO_FIELD(Object<OptionsDto>, options);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
