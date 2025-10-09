#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    ENUM(ExecutionErrorTypeDto, v_int8,
         VALUE(INVALID_ARGUMENT, 0, "InvalidArgument"),
         VALUE(INVALID_INPUT, 1, "InvalidInput"),
         VALUE(INVALID_FORMAT, 2, "InvalidFormat"),
         VALUE(INVALID_CONVERSION, 3, "InvalidConversion"),
         VALUE(EXECUTION_FAILED, 4, "ExecutionFailed"),
         VALUE(CONVERSION_FAILED, 5, "ConversionFailed"),
         VALUE(VALIDATION_FAILED, 6, "ValidationFailed"),
         VALUE(FILE_NOT_FOUND, 7, "FileNotFound")
    )

    class ErrorDto final : public oatpp::DTO
    {
        DTO_INIT(ErrorDto, DTO)

        DTO_FIELD(Enum<ExecutionErrorTypeDto>::AsString, type);
        DTO_FIELD(String, message);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
