#pragma once

#include "interop/types/execution_error.h"
#include "interop/types/execution_error_type.h"
#include "network/dto/execution/error_dto.hpp"

namespace engine::mapper
{
    class ErrorMapper final
    {
    public:
        static oatpp::Enum<network::dto::execution::ExecutionErrorTypeDto> to_dto(const interop::types::ExecutionErrorType& type)
        {
            return static_cast<network::dto::execution::ExecutionErrorTypeDto>(type);
        }

        static oatpp::Object<network::dto::execution::ErrorDto> to_dto(const interop::types::ExecutionError& error)
        {
            if (error.message == nullptr)
            {
                return nullptr;
            }

            auto dto = network::dto::execution::ErrorDto::createShared();
            dto->type = to_dto(error.type);
            dto->message = error.message;
            return dto;
        }
    };
} // namespace engine::mapper
