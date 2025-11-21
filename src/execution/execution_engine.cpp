/**
 * Project: Engine
 * File: execution_engine.cpp
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
 * @file execution_engine.cpp
 * @brief Implements the ExecutionEngine utility for executing scripts.
 */

#include "execution/execution_engine.hpp"

#include <string_view>

#include "execution/core_execution_result.hpp"
#include "execution/script/lua_script_runner.hpp"
#include "execution/script/lua_state_manager.hpp"
#include "interop/types/execution_error_type.h"
#include "interop/types/execution_metadata.h"
#include "interop/types/execution_status.h"
#include "interop/types/execution_type.h"
#include "utils/response_factory.hpp"

namespace
{
    /**
     * @brief Converts CoreExecutionErrorType to interop ExecutionErrorType.
     * @param type The core execution error type.
     * @return The corresponding interop execution error type.
     */
    engine::interop::types::ExecutionErrorType convert_error_type(engine::execution::CoreExecutionErrorType type)
    {
        switch (type)
        {
        case engine::execution::CoreExecutionErrorType::InvalidArgument:
            return engine::interop::types::InvalidArgument;
        case engine::execution::CoreExecutionErrorType::ExecutionFailed:
            return engine::interop::types::ExecutionFailed;
        case engine::execution::CoreExecutionErrorType::ValidationFailed:
            return engine::interop::types::ValidationFailed;
        case engine::execution::CoreExecutionErrorType::FileNotFound:
            return engine::interop::types::FileNotFound;
        default:
            return engine::interop::types::ExecutionFailed;
        }
    }

    /**
     * @brief Converts CoreExecutionStatus to interop ExecutionStatus.
     * @param status The core execution status.
     * @return The corresponding interop execution status.
     */
    engine::interop::types::ExecutionStatus convert_status(engine::execution::CoreExecutionStatus status)
    {
        switch (status)
        {
        case engine::execution::CoreExecutionStatus::Success:
            return engine::interop::types::Success;
        case engine::execution::CoreExecutionStatus::Failure:
            return engine::interop::types::Failure;
        case engine::execution::CoreExecutionStatus::Timeout:
            return engine::interop::types::Timeout;
        default:
            return engine::interop::types::Failure;
        }
    }

    /**
     * @brief Converts a CoreExectionResult to an interop ExecutionResponsePtr.
     * @param result The core execution result.
     * @return The corresponding interop execution response pointer.
     */
    engine::utils::ExecutionResponsePtr to_interop_response(const engine::execution::CoreExecutionResult& result)
    {
        const auto interop_status = convert_status(result.status);

        if (result.is_success())
        {
            engine::interop::types::ExecutionMetadata metadata;
            metadata.duration_milliseconds = result.metadata.duration_milliseconds;
            return engine::utils::ResponseFactory::create_success(metadata, result.string_output);
        }

        const auto interop_error_type = convert_error_type(result.error.type);
        return engine::utils::ResponseFactory::create_error(interop_status, interop_error_type, result.error.message);
    }
}

namespace engine::execution
{
    /**
     * @brief Executes a script based on the specified execution type. Returns an error response if the script is empty or the type is unsupported.
     * @param type The execution type.
     * @param script The script to execute.
     * @return A pointer to the execution response.
     */
    utils::ExecutionResponsePtr ExecutionEngine::execute(interop::types::ExecutionType type, std::string_view script)
    {
        if (script.empty())
        {
            return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                        interop::types::ExecutionErrorType::InvalidArgument,
                                                        "Execution script content is empty");
        }

        script::ScriptContext context;
        context.script_content = script;

        CoreExecutionResult result;
        const script::LuaScriptRunner runner(script::LuaStateManager::get_state());

        switch (type)
        {
        case interop::types::ExecutionType::LuaScript:
            result = runner.run_from_string(context);
            break;

        case interop::types::ExecutionType::LuaFile:
            result = runner.run_from_file(context);
            break;

        case interop::types::ExecutionType::Pipeline:
            // TODO: Implement pipeline execution
            return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                        interop::types::ExecutionErrorType::InvalidArgument,
                                                        "Pipeline execution not yet implemented");
        default:
            return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                        interop::types::ExecutionErrorType::InvalidArgument,
                                                        "Unsupported execution type");
        }

        return to_interop_response(result);
    }
} // namespace engine::execution
