#pragma once

#include "interop/types/execution_error.h"
#include "interop/types/execution_error_type.h"
#include "network/dto/execution/error_dto.hpp"

namespace engine::network::mapper
{
    class ErrorMapper final
    {
    public:
        ErrorMapper() = delete;

        static oatpp::Enum<dto::execution::ExecutionErrorTypeDto> to_dto(const interop::types::ExecutionErrorType& type)
        {
            return static_cast<dto::execution::ExecutionErrorTypeDto>(type);
        }

        static oatpp::Object<dto::execution::ErrorDto> to_dto(const interop::types::ExecutionError& error)
        {
            if (error.message == nullptr)
            {
                return nullptr;
            }

            auto dto = dto::execution::ErrorDto::createShared();
            dto->type = to_dto(error.type);
            dto->message = error.message;
            return dto;
        }
    };
} // namespace engine::network::mapper
