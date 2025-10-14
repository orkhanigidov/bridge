#include "execution/execution_engine.hpp"

#include "execution/script/script_executor.hpp"
#include "utils/response_factory.hpp"

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

        script::ScriptExecutor executor;

        switch (type)
        {
        case interop::types::ExecutionType::Lua_Script:
            return executor.execute_from_string(script);

        case interop::types::ExecutionType::Lua_File:
            {
                if (!fs::exists(script))
                {
                    return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                                interop::types::ExecutionErrorType::File_Not_Found,
                                                                ("Script file does not exist: " + script).c_str());
                }

                return executor.execute_from_file(script);
            }

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
    }
} // namespace engine::execution
