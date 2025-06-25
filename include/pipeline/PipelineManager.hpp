#pragma once

#include "pch.hpp"
#include "pipeline/Pipeline.hpp"

namespace engine::pipeline
{
    class PipelineManager
    {
      public:
        void load(std::string_view name, const dto::PipelineDTO& dto);
        void execute(std::string_view name) const;
        void clear();

      private:
        std::unordered_map<std::string, std::unique_ptr<Pipeline>> pipelines_;
    };
} // namespace engine::pipeline