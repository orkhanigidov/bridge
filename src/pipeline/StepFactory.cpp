#include "pipeline/StepFactory.hpp"

#include "pch.hpp"
#include "pipeline/CallStep.hpp"
#include "pipeline/NewStep.hpp"
#include "pipeline/Step.hpp"

namespace engine::pipeline
{
    std::unique_ptr<Step> StepFactory::create_step(const nlohmann::json& json)
    {
        validate_step_json(json);

        if (json.contains("new"))
        {
            return std::make_unique<NewStep>(json);
        }
        else if (json.contains("call"))
        {
            return std::make_unique<CallStep>(json);
        }

        throw std::invalid_argument("Unknown step type");
    }

    void StepFactory::validate_step_json(const nlohmann::json& json)
    {
        if (!json.is_object())
        {
            throw std::invalid_argument("Step must be a JSON object");
        }

        const bool has_new  = json.contains("new");
        const bool has_call = json.contains("call");

        if (!has_new && !has_call)
        {
            throw std::invalid_argument("Step must contain either 'new' or 'call' field");
        }

        if (has_new && has_call)
        {
            throw std::invalid_argument("Step cannot contain both 'new' and 'call' fields");
        }
    }
} // namespace engine::pipeline