#include "pipeline/PipelineManager.hpp"

#include "pch.hpp"
#include "pipeline/ObjectPool.hpp"
#include "pipeline/Pipeline.hpp"

namespace engine::pipeline
{
    void PipelineManager::load_pipeline(std::string_view name, const nlohmann::json& json)
    {
        pipelines_.emplace(name, std::make_unique<Pipeline>(json));
    }

    void PipelineManager::execute(std::string_view name)
    {
        if (pipelines_.find(std::string{name}) != pipelines_.end())
            pipelines_.at(std::string{name})->execute();

        clear_all();
    }

    void PipelineManager::clear_all()
    {
        pipelines_.clear();
        ObjectPool::instance().clear_all();
    }
} // namespace engine::pipeline