#pragma once

#include "Step.hpp"
#include "pch.hpp"

namespace engine::pipeline
{
    class StepFactory final
    {
      public:
        static std::unique_ptr<Step> create_step(const nlohmann::json& json);

      private:
        static void validate_step_json(const nlohmann::json& json);
    };
} // namespace engine::pipeline