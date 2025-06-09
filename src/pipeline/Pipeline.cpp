#include "pipeline/Pipeline.hpp"

#include "pch.hpp"

namespace engine::pipeline
{
    Pipeline::Pipeline(const nlohmann::json& json)
    {
        if (!json.contains("pipeline"))
        {
            throw std::invalid_argument("JSON must contain 'pipeline' field");
        }

        name_ = json["pipeline"].get<std::string>();

        if (json.contains("steps") && json["steps"].is_array())
        {
            commands_.reserve(json["steps"].size());

            for (const auto& step : json["steps"])
            {
                commands_.emplace_back(std::make_unique<Command>(step));
            }
        }
    }

    void Pipeline::execute()
    {
        for (auto& command : commands_)
        {
            command->execute();
        }
    }

    void Pipeline::clear()
    {
        commands_.clear();
    }
} // namespace engine::pipeline