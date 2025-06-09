#pragma once

#include "pch.hpp"
#include "pipeline/Pipeline.hpp"

namespace engine::pipeline
{
    class PipelineManager final
    {
      public:
        void load_pipeline(std::string_view name, const nlohmann::json& json);
        void execute(std::string_view name);
        void clear_all();

      private:
        std::unordered_map<std::string, std::unique_ptr<Pipeline>> pipelines_;
    };
} // namespace engine::pipeline