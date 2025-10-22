#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    ENUM(ExecutionErrorTypeDto, v_int8,
         VALUE(INVALID_ARGUMENT, 0, "InvalidArgument"),
         VALUE(EXECUTION_FAILED, 1, "ExecutionFailed"),
         VALUE(VALIDATION_FAILED, 2, "ValidationFailed"),
         VALUE(FILE_NOT_FOUND, 2, "FileNotFound")
    )
} // namespace engine::network::dto::execution
