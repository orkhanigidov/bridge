#include "pipeline/PipelineManager.hpp"

#include "dto/PipelineDTO.hpp"
#include "pch.hpp"
#include "pipeline/Pipeline.hpp"

namespace engine::pipeline
{
    void PipelineManager::load(std::string_view name, const dto::PipelineDTO& dto)
    {
        pipelines_.emplace(std::string(name), std::make_unique<Pipeline>(dto));
    }

    void PipelineManager::execute(std::string_view name) const
    {
        auto it = pipelines_.find(std::string(name));
        if (it != pipelines_.end())
        {
            it->second->execute();
        }
    }

    void PipelineManager::clear()
    {
        pipelines_.clear();
    }
} // namespace engine::pipeline