#include "engine/execution/script/script_executor.hpp"

#include "engine/lua_bindings/lua_registrar.hpp"
#include "pch.hpp"

namespace engine::execution
{
    void script_executor::execute_from_file(const std::filesystem::path& script_path)
    {
        const auto normalized_path = normalize_path(script_path);
        validate_path(normalized_path);

        auto& lua = lua_bindings::lua_registrar::instance().get_lua();

        try
        {
            const auto result = lua.safe_script_file(normalized_path);
            if (!result.valid())
            {
                const sol::error error = result;
                throw std::runtime_error("Lua script execution failed: " +
                                         std::string(error.what()));
            }
        }
        catch (const sol::error& e)
        {
            throw std::runtime_error("Error executing script '" + script_path.string() +
                                     "': " + e.what());
        }
    }

    void script_executor::execute_from_string(std::string_view script_content)
    {
        if (script_content.empty())
        {
            throw std::invalid_argument("Script content cannot be empty.");
        }

        auto& lua = lua_bindings::lua_registrar::instance().get_lua();

        try
        {
            const auto result = lua.safe_script(script_content);
            if (!result.valid())
            {
                const sol::error error = result;
                throw std::runtime_error("Lua script execution failed: " +
                                         std::string(error.what()));
            }
        }
        catch (const sol::error& e)
        {
            throw std::runtime_error("Error executing script from string: " +
                                     std::string(e.what()));
        }
    }

    std::filesystem::path script_executor::normalize_path(const std::filesystem::path& path)
    {
        auto normalized_path_str = path.string();
        std::replace(normalized_path_str.begin(), normalized_path_str.end(), "\\", "/");
        return std::filesystem::absolute(normalized_path_str);
    }

    void script_executor::validate_path(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path))
        {
            throw std::invalid_argument("Script path does not exist: " + path.string());
        }

        if (!std::filesystem::is_regular_file(path))
        {
            throw std::invalid_argument("Script path is not a regular file: " + path.string());
        }
    }
} // namespace engine::execution