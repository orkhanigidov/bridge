#pragma once

#include "interop/types/execution_type.h"
#include "utils/response_factory.hpp"

namespace engine::execution
{
    /**
     * @brief
     */
    class ExecutionEngine final
    {
    public:
        /**
         * @brief Deleted constructor to prevent instantiation of this static class
         */
        ExecutionEngine() = delete;

        /**
         * @brief
         * @param type
         * @param script
         * @return
         */
        static utils::ExecutionResponsePtr execute(interop::types::ExecutionType type, const std::string& script);
    };
} // namespace engine::execution
