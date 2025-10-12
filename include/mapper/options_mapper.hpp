#pragma once

#include "interop/interop_bridge.hpp"
#include "network/dto/execution/options_dto.hpp"

namespace engine::mapper
{
    class OptionsMapper final
    {
    public:
        static oatpp::Object<network::dto::execution::OptionsDto> to_dto(const interop::types::ExecutionOptions& options)
        {
            auto dto = network::dto::execution::OptionsDto::createShared();
            dto->timeout_milliseconds = options.timeout_milliseconds;
            dto->output_data_format = options.output_data_format;
            return dto;
        }

        static interop::types::ExecutionOptions from_dto(const oatpp::Object<network::dto::execution::OptionsDto>& dto)
        {
            interop::types::ExecutionOptions options{};
            if (dto)
            {
                options.timeout_milliseconds = dto->timeout_milliseconds;
                options.output_data_format = dto->output_data_format->c_str();
            }
            return options;
        }
    };
} // namespace engine::mapper
