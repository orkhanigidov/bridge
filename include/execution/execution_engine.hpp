/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file execution_engine.hpp
 * @brief Declares the ExecutionEngine utility for executing scripts.
 */

#pragma once

#include <string_view>

#include "interop/types/execution_type.h"
#include "utils/response_factory.hpp"

namespace engine::execution
{
    /**
     * @class ExecutionEngine
     * @brief Utility class for executing scripts based on the execution type.
     */
    class ExecutionEngine final
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        ExecutionEngine() = delete;

        /**
         * @brief Executes a script based on the specified execution type.
         * @param type The execution type.
         * @param script The script to execute.
         * @return A pointer to the execution response.
         */
        static utils::ExecutionResponsePtr execute(interop::types::ExecutionType type, std::string_view script);
    };
} // namespace engine::execution
