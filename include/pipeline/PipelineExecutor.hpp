#pragma once

#include "pch.hpp"
#include "pipeline/Pipeline.hpp"

namespace engine::pipeline
{
    class PipelineExecutor final
    {
      public:
        static PipelineExecutor& instance();

        void load_json(const nlohmann::json& json);
        void execute();
        void clear();

      private:
        std::unique_ptr<Pipeline> pipeline_;

        PipelineExecutor() = default;
    };
} // namespace engine::pipeline