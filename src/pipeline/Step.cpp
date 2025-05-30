#include "pipeline/Step.hpp"

#include "pch.hpp"

namespace engine::pipeline
{
    Step::Step(const nlohmann::json& json)
    {
        if (json.contains("new"))
        {
            type_ = StepType::New;
            name_ = json["new"].get<std::string>();
        }
        else if (json.contains("call"))
        {
            type_ = StepType::Call;
            name_ = json["call"].get<std::string>();
        }
        else
            throw std::invalid_argument("Step must contain either 'new' or 'call' field");


        if (json.contains("as"))
            alias_ = json["as"].get<std::string>();

        if (json.contains("with"))
        {
            const auto& with_obj = json["with"];
            if (with_obj.is_object())
            {
                for (const auto& [key, value] : with_obj.items())
                {
                    parameters_[key] = value;
                }
            }
        }
    }
} // namespace engine::pipeline