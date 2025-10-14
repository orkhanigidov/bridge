#pragma once

#include "sol/protected_function.hpp"
#include "sol/state.hpp"
#include "utils/response_factory.hpp"

namespace engine::execution::script
{
    class ScriptExecutor final
    {
    public:
        ScriptExecutor();

        utils::ExecutionResponsePtr execute_from_file(const fs::path& script_path);
        utils::ExecutionResponsePtr execute_from_string(const std::string& script_content);

    private:
        sol::state lua_;

        utils::ExecutionResponsePtr execute_lua(const std::function<sol::protected_function_result()>& script_runner);
    };
} // namespace engine::execution::script
