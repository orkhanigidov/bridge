#include "pipeline/Pipeline.hpp"

#include "pch.hpp"
#include "pipeline/StepFactory.hpp"

namespace engine::pipeline
{
    Pipeline::Pipeline(const nlohmann::json& json)
    {
        if (!json.contains("pipeline"))
        {
            throw std::invalid_argument("JSON must contain 'pipeline' field");
        }

        name_ = json["pipeline"].get<std::string>();

        if (json.contains("steps"))
        {
            load_steps(json["steps"]);
        }
    }

    void Pipeline::execute()
    {
        for (auto& step : steps_)
        {
            step->execute();
        }
    }

    void Pipeline::clear()
    {
        steps_.clear();
    }

    void Pipeline::load_steps(const nlohmann::json& steps_json)
    {
        if (!steps_json.is_array())
        {
            throw std::invalid_argument("Steps must be an array");
        }

        steps_.reserve(steps_json.size());

        for (const auto& step_json : steps_json)
        {
            steps_.emplace_back(StepFactory::create_step(step_json));
        }
    }
} // namespace engine::pipeline