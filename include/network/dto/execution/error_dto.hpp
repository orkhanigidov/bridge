#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    ENUM(ExecutionErrorType, v_int8,
         VALUE(INVALID_ARGUMENT, 0, "invalid_argument"),
         VALUE(INVALID_INPUT, 1, "invalid_input"),
         VALUE(INVALID_FORMAT, 2, "invalid_format"),
         VALUE(INVALID_CONVERSION, 3, "invalid_conversion"),
         VALUE(EXECUTION_FAILED, 4, "execution_failed"),
         VALUE(CONVERSION_FAILED, 5, "conversion_failed"),
         VALUE(VALİDATİON_FAILED, 6, "validation_failed"),
         VALUE(FILE_NOT_FOUND, 7, "file_not_found")
    )

    class ErrorDto final : public oatpp::DTO
    {
        DTO_INIT(ErrorDto, DTO)

        DTO_FIELD(Enum<ExecutionErrorType>::AsString, type);
        DTO_FIELD(String, message);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
