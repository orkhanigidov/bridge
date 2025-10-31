/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <oatpp/core/Types.hpp>

#include "options_mapper.hpp"
#include "interop/types/execution_request.h"
#include "interop/types/execution_type.h"
#include "network/dto/execution/file_dto.hpp"
#include "network/dto/execution/request_dto.hpp"
#include "network/dto/execution/type_dto.hpp"

namespace engine::network::mapper
{
    class RequestMapper final
    {
    public:
        RequestMapper() = delete;

        static oatpp::Enum<dto::execution::ExecutionTypeDto> to_dto(const interop::types::ExecutionType& type)
        {
            return static_cast<dto::execution::ExecutionTypeDto>(type);
        }

        static oatpp::Object<dto::execution::RequestDto> to_dto(const interop::types::ExecutionRequest& request,
                                                                const oatpp::Object<dto::execution::FileDto>& input_data)
        {
            auto dto = dto::execution::RequestDto::createShared();
            dto->type = to_dto(request.type);
            dto->script = request.script;
            dto->input_data = input_data;
            dto->options = OptionsMapper::to_dto(request.options);
            return dto;
        }
    };
} // namespace engine::network::mapper
