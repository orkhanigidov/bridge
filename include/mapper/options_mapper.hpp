#pragma once

#include "interop/interop_bridge.hpp"
#include "network/dto/execution/options_dto.hpp"

namespace engine::mapper
{
    using namespace engine::network::dto::execution;
    using namespace engine::interop::types;

    class OptionsMapper final
    {
    public:
        static oatpp::Object<OptionsDto> to_dto(const ExecutionOptions& options)
        {
            auto dto = OptionsDto::createShared();
            dto->timeout_milliseconds = options.timeout_milliseconds;
            dto->output_data_format = options.output_data_format;
            return dto;
        }

        static ExecutionOptions from_dto(const oatpp::Object<OptionsDto>& dto)
        {
            ExecutionOptions options;
            options.timeout_milliseconds = dto->timeout_milliseconds;
            options.output_data_format = dto->output_data_format->c_str();
            return options;
        }
    };
} // namespace engine::mapper
