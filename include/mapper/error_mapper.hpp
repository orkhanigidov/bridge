#pragma once

#include "interop/interop_bridge.hpp"
#include "network/dto/execution/error_dto.hpp"

namespace engine::mapper
{
    using namespace engine::network::dto::execution;
    using namespace engine::interop::types;

    class ErrorMapper final
    {
    public:
        static oatpp::Enum<ExecutionErrorTypeDto> to_dto(const ExecutionErrorType& type)
        {
            return static_cast<ExecutionErrorTypeDto>(type);
        }

        static oatpp::Object<ErrorDto> to_dto(const ExecutionError& error)
        {
            if (error.message == nullptr)
            {
                return nullptr;
            }

            auto dto = ErrorDto::createShared();
            dto->type = to_dto(error.type);
            dto->message = error.message;
            return dto;
        }
    };
} // namespace engine::mapper
