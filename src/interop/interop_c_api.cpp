/**
 * Project: Engine
 * File: interop_c_api.cpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file interop_c_api.cpp
 * @brief Implements the C API for script execution and response management.
 */

#include "interop/interop_c_api.hpp"

#include "interop/interop_bridge.hpp"
#include "interop/types/execution_error_type.h"
#include "interop/types/execution_status.h"
#include "utils/response_factory.hpp"

/**
 * @brief Initializes the script bindings and prewarms the Lua state.
 * @return True if initialization succeeds, false otherwise.
 */
bool InitializeBindings()
{
    return engine::interop::prewarm_thread_state();
}

/**
 * @brief Executes a script based on the provided execution request.
 * @param request Pointer to the execution request.
 * @return Pointer to the execution response. Returns an error response if the request is null.
 */
ExecutionResponse* ExecuteScript(const ExecutionRequest* request)
{
    if (!request)
    {
        return engine::utils::ResponseFactory::create_error(engine::interop::types::ExecutionStatus::Failure,
                                                            engine::interop::types::ExecutionErrorType::InvalidArgument,
                                                            "The provided request is null.").release();
    }

    return engine::interop::execute(request).release();
}

/**
 * @brief Frees the memory allocated for an execution response.
 * @param response Pointer to the execution response to free.
 */
void FreeExecutionResponse(ExecutionResponse* response)
{
    if (!response)
    {
        return;
    }

    engine::utils::ExecutionResponseDeleter deleter;
    deleter(response);
}
