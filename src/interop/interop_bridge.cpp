/**
 * Project: Engine
 * File: interop_bridge.cpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file interop_bridge.cpp
 * @brief Implements the interop bridge for script execution and Lua state management.
 */

#include "interop/interop_bridge.hpp"

#include <exception>
#include <iostream>

#include "execution/execution_engine.hpp"
#include "execution/script/lua_script_runner.hpp"
#include "execution/script/lua_state_manager.hpp"
#include "interop/interop_c_api.hpp"
#include "interop/types/execution_error_type.h"
#include "interop/types/execution_status.h"
#include "interop/types/execution_type.h"
#include "utils/response_factory.hpp"

namespace engine::interop
{
    /**
     * @brief Pre-warms the thread-local Lua state for script execution.
     * @return True if successful, false otherwise.
     *
     * Logs errors to std::cerr if Lua state initialization fails.
     */
    bool prewarm_thread_state()
    {
        try
        {
            execution::script::LuaStateManager::get_state();
            return true;
        } catch (const std::exception& e)
        {
            std::cerr << "Failed to pre-warm thread-local Lua state: " << e.what() << std::endl;
            return false;
        }
    }

    /**
     * @brief Executes a script based on the provided execution request.
     * @param request Pointer to the exection request.
     * @return Pointer to the exection response.
     *
     * Returns an error response for unsupported execution types or exceptions.
     */
    utils::ExecutionResponsePtr execute(const ExecutionRequest* request)
    {
        try
        {
            switch (request->type)
            {
            case types::ExecutionType::LuaScript:
            case types::ExecutionType::LuaFile:
                return execution::ExecutionEngine::execute(request->type, request->script);
            default:
                return utils::ResponseFactory::create_error(types::ExecutionStatus::Failure,
                                                            types::ExecutionErrorType::InvalidArgument,
                                                            "Unsupported execution type");
            }
        } catch (const std::exception& e)
        {
            return utils::ResponseFactory::create_error(types::ExecutionStatus::Failure,
                                                        types::ExecutionErrorType::ExecutionFailed,
                                                        e.what());
        }
    }
} // namespace engine::interop
