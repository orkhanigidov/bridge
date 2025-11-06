/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file execution_service.hpp
 * @brief Declares the ExecutionService utility for executing requests.
 */

#pragma once

#include <format>
#include <stdexcept>
#include <utility>
#include <oatpp/core/Types.hpp>

#include "core_execution_result.hpp"
#include "network/dto/execution/request_dto.hpp"

namespace engine::execution
{
    /**
     * @class ExecutionServiceException
     * @brief Exception type for errors occurring in ExecutionService.
     */
    class ExecutionServiceException final : public std::runtime_error
    {
    public:
        /**
         * @brief Constructs an ExecutionServiceException with a formatted message.
         * @tparam Args Argument types for formatting.
         * @param fmt The format string.
         * @param args Arguments for the format string.
         */
        template <typename... Args>
        explicit ExecutionServiceException(std::format_string<Args...> fmt, Args&&... args)
            : std::runtime_error(std::format(fmt, std::forward<Args>(args)...))
        {
        }
    };

    /**
     * @class ExecutionService
     * @brief Utility class for executing requests.
     */
    class ExecutionService
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        ExecutionService() = delete;

        /**
         * @brief Executes a request and returns the result.
         * @param request The execution request DTO.
         * @return The result of the execution.
         */
        static CoreExecutionResult execute(const oatpp::Object<network::dto::execution::RequestDto>& request);
    };
}
