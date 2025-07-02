#include "engine/execution/pipeline/pipeline_executor.hpp"

#include "engine/dto/pipeline.hpp"
#include "engine/execution/pipeline/pipeline_step.hpp"

namespace engine::pipeline
{
    void pipeline_executor::execute(const dto::pipeline& pipeline) const
    {
        if (!pipeline.steps || pipeline.steps->empty())
        {
            throw std::invalid_argument("Pipeline must contain at least one step");
        }

        execute_pipeline_steps(*pipeline.steps);
    }

    void pipeline_executor::execute_pipeline_steps(const std::list<dto::pipeline_step>& steps) const
    {
        for (const auto& step : steps)
        {
            pipeline_step pipeline_step{step};
            pipeline_step.execute();
        }
    }
} // namespace engine::pipeline