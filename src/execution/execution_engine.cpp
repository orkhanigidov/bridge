#include "engine/execution/execution_engine.hpp"

#include "engine/execution/script/script_executor.hpp"

namespace engine::execution
{
    ExecutionEngine::ExecutionEngine(ExecutionType exec_type)
    {
        if (exec_type == ExecutionType::Json_Pipeline)
        {
            m_pipeline_executor = std::make_unique<pipeline::PipelineExecutor>();
        }
        else
        {
            m_script_executor = std::make_unique<script::ScriptExecutor>();
        }
    }

    void ExecutionEngine::execute_script(const std::string& script) const
    {
        if (!m_script_executor)
        {
            throw std::runtime_error("Script executor is not initialized");
        }

        if (script.empty())
        {
            throw std::invalid_argument("Script content cannot be empty");
        }

        m_script_executor->execute_from_string(script);
    }

    void ExecutionEngine::execute_script_file(const std::filesystem::path& path) const
    {
        if (!m_script_executor)
        {
            throw std::runtime_error("Script executor is not initialized");
        }

        if (!is_valid_path(path))
        {
            throw std::invalid_argument("Invalid script file path: " + path.string());
        }

        m_script_executor->execute_from_file(path);
    }

    void ExecutionEngine::execute_pipeline(const std::string& json) const
    {
        if (!m_pipeline_executor)
        {
            throw std::runtime_error("Pipeline executor is not initialized");
        }

        if (json.empty())
        {
            throw std::invalid_argument("JSON content cannot be empty");
        }

        // m_pipeline_executor->execute_from_string(json);
    }

    void ExecutionEngine::execute_pipeline_file(const fs::path& path) const
    {
        if (!m_pipeline_executor)
        {
            throw std::runtime_error("Pipeline executor is not initialized");
        }

        if (!is_valid_path(path))
        {
            throw std::invalid_argument("Invalid json pipeline path: " + path.string());
        }

        // m_pipeline_executor->execute_from_file(path);
    }

    bool ExecutionEngine::is_valid_path(const fs::path& file_path) const
    {
        if (!fs::exists(file_path))
        {
            std::cerr << "File does not exist: " << file_path << std::endl;
            return false;
        }

        if (!fs::is_regular_file(file_path))
        {
            std::cerr << "File is not a regular file: " << file_path << std::endl;
            return false;
        }
        return true;
    }
} // namespace engine::execution
