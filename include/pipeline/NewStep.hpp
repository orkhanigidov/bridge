#pragma once

#include "Step.hpp"
#include "pch.hpp"

namespace engine::pipeline
{
    class NewStep final : public Step
    {
      public:
        explicit NewStep(const nlohmann::json& json);

        void execute() override;

      private:
        std::string class_name_;
    };
} // namespace engine::pipeline