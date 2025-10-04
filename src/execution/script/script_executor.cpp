#include "execution/script/script_executor.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "bindings/lua/registry.hpp"

namespace engine::execution::script {

    interop::types::ExecutionResponse ScriptExecutor::execute_from_file(const std::filesystem::path& script_path)
    {
        const auto normalized_path = normalize_path(script_path);

        auto& lua = bindings::lua::Registry::instance().lua();

        interop::types::ExecutionResponse result;
        auto start = std::chrono::high_resolution_clock::now();

        try {
            const auto lua_result = lua.safe_script_file(normalized_path.string());
            if (!lua_result.valid()) {
                const sol::error error = lua_result;
                result.status = interop::types::ExecutionStatus::Failure;
                result.error.message = ("Lua script execution failed: " + std::string(error.what())).c_str();
            }
            else {
                result.status = interop::types::ExecutionStatus::Success;
            }
        }
        catch (const sol::error& e) {
            result.status = interop::types::ExecutionStatus::Failure;
            result.error.message = ("Error executing script '" + script_path.string() + "': " + e.what()).c_str();
        }

        auto end = std::chrono::high_resolution_clock::now();
        result.metadata.duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        return result;
    }

    interop::types::ExecutionResponse ScriptExecutor::execute_from_string(const std::string& script_content)
    {
        auto& lua = bindings::lua::Registry::instance().lua();

        interop::types::ExecutionResponse result;
        auto start = std::chrono::high_resolution_clock::now();

        try {
            const auto lua_result = lua.safe_script(script_content);
            if (!lua_result.valid()) {
                const sol::error error = lua_result;
                result.status = interop::types::ExecutionStatus::Failure;
                result.error.message = ("Lua script execution failed: " + std::string(error.what())).c_str();
            }
            else {
                result.status = interop::types::ExecutionStatus::Success;
            }
        }
        catch (const sol::error& e) {
            result.status = interop::types::ExecutionStatus::Failure;
            result.error.message = ("Error executing script from string: " + std::string(e.what())).c_str();
        }

        auto end = std::chrono::high_resolution_clock::now();
        result.metadata.duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        return result;
    }

    fs::path ScriptExecutor::normalize_path(const fs::path& path)
    {
        auto normalized = path.string();
        std::ranges::replace(normalized, '\\', '/');
        return fs::absolute(normalized);
    }

} // namespace engine::execution::script
