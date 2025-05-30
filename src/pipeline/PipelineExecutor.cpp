#include "pipeline/PipelineExecutor.hpp"

#include "pipeline/ObjectPool.hpp"
#include "pipeline/Pipeline.hpp"

namespace engine::pipeline
{
    PipelineExecutor& PipelineExecutor::instance()
    {
        static PipelineExecutor instance;
        return instance;
    }

    void PipelineExecutor::load_json(const nlohmann::json& json)
    {
        clear();
        pipeline_ = std::make_unique<Pipeline>(json);
    }

    void PipelineExecutor::execute()
    {
        if (!pipeline_)
        {
            throw std::runtime_error("No pipeline loaded");
        }

        pipeline_->execute();
    }

    void PipelineExecutor::clear()
    {
        pipeline_.reset();
        ObjectPool::instance().clear_all();
    }
} // namespace engine::pipeline