#pragma once

#include "CallStep.hpp"
#include "Step.hpp"
#include "pch.hpp"

namespace engine::pipeline
{
    class CallStep final : public Step
    {
      public:
        explicit CallStep(const nlohmann::json& json);

        void execute() override;

      private:
        std::string function_name_;
        std::optional<std::string> instance_name_;

        bool is_method_call() const noexcept;
        void parse_call_name();
    };
} // namespace engine::pipeline