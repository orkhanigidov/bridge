#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    ENUM(ExecutionStatusDto, v_int8,
         VALUE(SUCCESS, 0, "Success"),
         VALUE(FAILURE, 1, "Failure"),
         VALUE(TIMEOUT, 2, "Timeout")
    )
} // namespace engine::network::dto::execution
