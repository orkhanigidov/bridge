#pragma once

#include "options_mapper.hpp"
#include "network/dto/execution/request_dto.hpp"

namespace engine::mapper
{
    class RequestMapper final
    {
    public:
        static oatpp::Enum<network::dto::execution::ExecutionTypeDto> to_dto(const interop::types::ExecutionType& type)
        {
            return static_cast<network::dto::execution::ExecutionTypeDto>(type);
        }

        static oatpp::Object<network::dto::execution::RequestDto> to_dto(const interop::types::ExecutionRequest& request,
                                                                         const oatpp::Object<network::dto::execution::FileDto>& input_data)
        {
            auto dto = network::dto::execution::RequestDto::createShared();
            dto->type = to_dto(request.type);
            dto->script = request.script;
            dto->input_data = input_data;
            dto->options = OptionsMapper::to_dto(request.options);
            return dto;
        }
    };
} // namespace engine::mapper
