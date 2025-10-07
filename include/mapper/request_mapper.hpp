#pragma once

#include "options_mapper.hpp"
#include "network/dto/execution/request_dto.hpp"

namespace engine::mapper
{
    using namespace engine::network::dto::execution;
    using namespace engine::interop::types;

    class ResponseMapper final
    {
    public:
        static oatpp::Enum<ExecutionTypeDto> to_dto(const ExecutionType& type)
        {
            return static_cast<ExecutionTypeDto>(type);
        }

        static oatpp::Object<RequestDto> to_dto(const ExecutionRequest& request, const oatpp::Object<FileDto>& input_data)
        {
            auto dto = RequestDto::createShared();
            dto->type = to_dto(request.type);
            dto->script = request.script;
            dto->input_data = input_data;
            dto->options = OptionsMapper::to_dto(request.options);
            return dto;
        }
    };
} // namespace engine::mapper
