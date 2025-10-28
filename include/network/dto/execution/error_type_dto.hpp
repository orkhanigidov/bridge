#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    ENUM(ExecutionErrorTypeDto, v_int8,
         VALUE(InvalidArgument, 0, "InvalidArgument"),
         VALUE(ExecutionFailed, 1, "ExecutionFailed"),
         VALUE(ValidationFailed, 2, "ValidationFailed"),
         VALUE(FileNotFound, 2, "FileNotFound")
    )
} // namespace engine::network::dto::execution
