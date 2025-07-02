#pragma once

#include "engine/execution/pipeline/pipeline_executor.hpp"
#include "engine/execution/script/script_executor.hpp"
#include "pch.hpp"

namespace engine::execution
{
    class execution_engine final
    {
      public:
        static execution_engine& instance()
        {
            static execution_engine instance;
            return instance;
        }

        void execute_pipeline(std::string_view json);
        void execute_pipeline_file(const std::filesystem::path& path);

        void execute_script(std::string_view script);
        void execute_script_file(const std::filesystem::path& path);

      private:
        std::unique_ptr<pipeline::pipeline_executor> pipeline_executor_;
        std::unique_ptr<script_executor> script_executor_;

        void validate_path(const std::filesystem::path& path) const;
    };
} // namespace engine::execution