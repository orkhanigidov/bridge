#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    ENUM(ExecutionStatusDto, v_int8,
         VALUE(Success, 0, "Success"),
         VALUE(Failure, 1, "Failure"),
         VALUE(Timeout, 2, "Timeout")
    )
} // namespace engine::network::dto::execution
