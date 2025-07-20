#pragma once

#include "execution_type.hpp"
#include "engine/execution/pipeline/pipeline_executor.hpp"
#include "engine/execution/script/script_executor.hpp"

namespace engine::execution
{
    class ExecutionEngine final
    {
    public:
        static ExecutionEngine& instance()
        {
            static ExecutionEngine instance;
            return instance;
        }

        explicit ExecutionEngine(ExecutionType exec_type = ExecutionType::Lua_Script);

        void execute_script(const std::string& script) const;
        void execute_script_file(const fs::path& path) const;
        void execute_pipeline(const std::string& json) const;
        void execute_pipeline_file(const fs::path& path) const;

    private:
        std::unique_ptr<script::ScriptExecutor> m_script_executor;
        std::unique_ptr<pipeline::PipelineExecutor> m_pipeline_executor;

        bool is_valid_path(const fs::path& path) const;
    };
} // namespace engine::execution
