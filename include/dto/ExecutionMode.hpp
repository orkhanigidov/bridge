#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    ENUM(ExecutionMode, v_int8, VALUE(Native, 0, "Native"), VALUE(Network, 1, "Network"))

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto