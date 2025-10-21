#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    ENUM(ExecutionTypeDto, v_int8,
         VALUE(LUA_SCRIPT, 0, "LuaScript"),
         VALUE(PIPELINE, 1, "Pipeline")
    )
} // namespace engine::network::dto::execution
