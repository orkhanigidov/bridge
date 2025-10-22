#pragma once

#include "core_execution_result.hpp"
#include "network/dto/execution/request_dto.hpp"

#include <oatpp/core/Types.hpp>

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

    /**
     * @brief
     */
    class ExecutionService
    {
    public:
        /**
         * @brief Deleted constructor to prevent instantiation of this static class
         */
        ExecutionService() = delete;

        /**
         * @brief
         * @param request
         * @return
         */
        static CoreExecutionResult execute(const oatpp::Object<network::dto::execution::RequestDto>& request);
    };
}
