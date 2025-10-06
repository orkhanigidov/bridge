#include "execution/execution_engine.hpp"

#include "execution/script/script_executor.hpp"
#include "utils/response_factory.hpp"

namespace engine::execution
{

    utils::ExecutionResponsePtr ExecutionEngine::execute_lua(interop::types::ExecutionType type,
                                                             const std::string& script_or_path)
    {
        if (script_or_path.empty())
        {
            return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                        interop::types::ExecutionErrorType::Invalid_Argument,
                                                        "Script is empty");
        }

        script::ScriptExecutor executor;

        switch (type)
        {
            case interop::types::ExecutionType::Lua_Script:
                return executor.execute_from_string(script_or_path);
            case interop::types::ExecutionType::Lua_Script_File:
            {
                if (!fs::exists(script_or_path))
                {
                    return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                                interop::types::ExecutionErrorType::File_Not_Found,
                                                                "File does not exist");
                }

                if (!fs::is_regular_file(script_or_path))
                {
                    return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                                interop::types::ExecutionErrorType::Invalid_Argument,
                                                                "Path is not a regular file");
                }

                return executor.execute_from_file(script_or_path);
            }
            default:
                return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                            interop::types::ExecutionErrorType::Invalid_Argument,
                                                            "Invalid execution type");
        }
    }

    utils::ExecutionResponsePtr ExecutionEngine::execute_pipeline(interop::types::ExecutionType type,
                                                                  const std::string& json_or_path)
    {
        return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                    interop::types::ExecutionErrorType::Invalid_Argument,
                                                    "Pipeline execution not implemented");
    }

} // namespace engine::execution
