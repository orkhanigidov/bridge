#pragma once

#include "pch.hpp"

namespace engine::pipeline
{
    enum class StepType : std::uint8_t
    {
        New,
        Call
    };

    class Step
    {
      public:
        explicit Step(const nlohmann::json& json);
        virtual ~Step() = default;

        StepType type() const noexcept
        {
            return type_;
        }

        std::string_view name() const noexcept
        {
            return name_;
        }

        const std::optional<std::string_view> alias() const noexcept
        {
            return alias_;
        }

        virtual void execute() = 0;

      protected:
        StepType type_;
        std::string name_;
        std::optional<std::string> alias_;
        std::unordered_map<std::string, nlohmann::json> parameters_;
    };
} // namespace engine::pipeline