#include "engine/execution/execution_engine.hpp"

#include "engine/execution/script/script_executor.hpp"
#include "pch.hpp"

namespace engine::execution
{
    void execution_engine::execute_pipeline(std::string_view json)
    {
        if (json.empty())
        {
            throw std::invalid_argument("JSON content cannot be empty");
        }

        if (!pipeline_executor_)
        {
            pipeline_executor_ = std::make_unique<pipeline::pipeline_executor>();
        }

        try
        {
            pipeline_executor_->execute(json);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Pipeline execution failed: " + std::string(e.what()));
        }
    }

    void execution_engine::execute_pipeline_file(const std::filesystem::path& path)
    {
        validate_path(path);

        try
        {
            execute_pipeline(content);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Pipeline file execution failed: " + std::string(e.what()));
        }
    }

    void execution_engine::execute_script(std::string_view script)
    {
        if (script.empty())
        {
            throw std::invalid_argument("Script content cannot be empty");
        }

        if (!script_executor_)
        {
            script_executor_ = std::make_unique<script_executor>();
        }

        try
        {
            script_executor_->execute_from_string(script);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Script execution failed: " + std::string(e.what()));
        }
    }

    void execution_engine::execute_script_file(const std::filesystem::path& path)
    {
        validate_path(path);

        if (!script_executor_)
        {
            script_executor_ = std::make_unique<script_executor>();
        }

        try
        {
            script_executor_->execute_from_file(path);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Script file execution failed: " + std::string(e.what()));
        }
    }

    void execution_engine::validate_path(const std::filesystem::path& path) const
    {
        if (!std::filesystem::exists(path))
        {
            throw std::invalid_argument("File does not exist: " + path.string());
        }

        if (!std::filesystem::is_regular_file(path))
        {
            throw std::invalid_argument("Path is not a regular file: " + path.string());
        }
    }
} // namespace engine::execution