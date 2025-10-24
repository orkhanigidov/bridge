#pragma once

#include <oatpp/core/Types.hpp>

#include "interop/types/execution_options.h"
#include "network/dto/execution/options_dto.hpp"

namespace engine::network::mapper
{
    class OptionsMapper final
    {
    public:
        OptionsMapper() = delete;

        static oatpp::Object<dto::execution::OptionsDto> to_dto(const interop::types::ExecutionOptions& options)
        {
            auto dto = dto::execution::OptionsDto::createShared();
            dto->timeout_milliseconds = options.timeout_milliseconds;
            dto->output_data_format = options.output_data_format;
            return dto;
        }

        static interop::types::ExecutionOptions from_dto(const oatpp::Object<dto::execution::OptionsDto>& dto)
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
} // namespace engine::network::mapper
