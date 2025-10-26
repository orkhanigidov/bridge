#include "execution/execution_engine.hpp"

#include <string>

#include "execution/core_execution_result.hpp"
#include "execution/script/lua_script_runner.hpp"
#include "interop/types/execution_error_type.h"
#include "interop/types/execution_metadata.h"
#include "interop/types/execution_status.h"
#include "interop/types/execution_type.h"
#include "utils/response_factory.hpp"

namespace
{
    engine::interop::types::ExecutionErrorType convert_error_type(engine::execution::CoreExecutionErrorType type)
    {
        using enum engine::execution::CoreExecutionErrorType;

        switch (type)
        {
        case Invalid_Argument:
            return engine::interop::types::Invalid_Argument;
        case Execution_Failed:
            return engine::interop::types::Execution_Failed;
        case Validation_Failed:
            return engine::interop::types::Validation_Failed;
        case File_Not_Found:
            return engine::interop::types::File_Not_Found;
        default:
            return engine::interop::types::Execution_Failed;
        }
    }

    engine::interop::types::ExecutionStatus convert_status(engine::execution::CoreExecutionStatus status)
    {
        using enum engine::execution::CoreExecutionStatus;

        switch (status)
        {
        case Success:
            return engine::interop::types::Success;
        case Failure:
            return engine::interop::types::Failure;
        case Timeout:
            return engine::interop::types::Timeout;
        default:
            return engine::interop::types::Failure;
        }
    }

    engine::utils::ExecutionResponsePtr to_interop_response(const engine::execution::CoreExecutionResult& result)
    {
        const auto interop_status = convert_status(result.status);

        if (result.is_success())
        {
            engine::interop::types::ExecutionMetadata metadata;
            metadata.duration_milliseconds = result.metadata.duration_milliseconds;
            return engine::utils::ResponseFactory::create_success(metadata);
        }

        const auto interop_error_type = convert_error_type(result.error.type);
        return engine::utils::ResponseFactory::create_error(interop_status, interop_error_type, result.error.message);
    }
}

namespace engine::execution
{
    utils::ExecutionResponsePtr ExecutionEngine::execute(interop::types::ExecutionType type, const std::string& script)
    {
        if (script.empty())
        {
            return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                        interop::types::ExecutionErrorType::Invalid_Argument,
                                                        "Execution script content is empty");
        }

        script::ScriptContext context;
        context.script_content = script;

        CoreExecutionResult result;
        const script::LuaScriptRunner runner;

        switch (type)
        {
        case interop::types::ExecutionType::Lua_Script:
            result = runner.run_from_string(context);
            break;

        case interop::types::ExecutionType::Lua_File:
            result = runner.run_from_file(context);
            break;

        case interop::types::ExecutionType::Pipeline:
            // TODO: Implement pipeline execution
            return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                        interop::types::ExecutionErrorType::Invalid_Argument,
                                                        "Pipeline execution not yet implemented");
        default:
            return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                        interop::types::ExecutionErrorType::Invalid_Argument,
                                                        "Unsupported execution type");
        }

        return to_interop_response(result);
    }
} // namespace engine::execution
