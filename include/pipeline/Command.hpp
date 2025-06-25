#pragma once

#include "dto/StepDTO.hpp"
#include "oatpp/core/Types.hpp"
#include "pch.hpp"
#include "serialization/Converter.hpp"

namespace engine::pipeline
{
    class Command
    {
      public:
        explicit Command(const dto::StepDTO& dto);

        void execute() const;

      private:
        std::string name_;
        std::optional<std::string> alias_;
        std::vector<serialization::Value> parameters_;

        void execute_constructor() const;
        void execute_member_function() const;
        void execute_global_function() const;

        std::vector<sol::object> resolve_params(sol::state& lua) const;

        bool is_constructor() const noexcept;
        bool is_member_function() const noexcept;
    };
} // namespace engine::pipeline