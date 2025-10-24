#pragma once

#include <format>
#include <stdexcept>
#include <utility>
#include <oatpp/core/Types.hpp>

#include "core_execution_result.hpp"
#include "network/dto/execution/request_dto.hpp"

namespace engine::execution
{
    class ExecutionServiceException final : public std::runtime_error
    {
    public:
        template <typename... Args>
        explicit ExecutionServiceException(std::format_string<Args...> fmt, Args&&... args)
            : std::runtime_error(std::format(fmt, std::forward<Args>(args)...))
        {
        }
    };

    class ExecutionService
    {
    public:
        ExecutionService() = delete;

        static CoreExecutionResult execute(const oatpp::Object<network::dto::execution::RequestDto>& request);
    };
}
