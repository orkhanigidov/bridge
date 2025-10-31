/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#include "interop/interop_bridge.hpp"

#include <exception>
#include <format>
#include <iostream>
#include <mutex>

#include "execution/execution_engine.hpp"
#include "execution/script/lua_script_runner.hpp"
#include "interop/interop_c_api.hpp"
#include "interop/types/execution_error_type.h"
#include "interop/types/execution_status.h"
#include "interop/types/execution_type.h"
#include "utils/response_factory.hpp"

namespace engine::interop
{
    bool prewarm_thread_state()
    {
        try
        {
            execution::script::LuaStateManager::get_state();
            return true;
        } catch (const std::exception& e)
        {
            std::cerr << std::format("Failed to pre-warm thread-local Lua state: {}", e.what());
            return false;
        }
    }

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
