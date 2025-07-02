#pragma once

#include "engine/dto/pipeline_step.hpp"
#include "engine/serialization/oatpp_type_adapter.hpp"
#include "pch.hpp"

namespace engine::pipeline
{
    class pipeline_step final
    {
      public:
        explicit pipeline_step(const dto::pipeline_step& dto);

        void execute() const;

      private:
        std::string name_;
        std::optional<std::string> alias_;
        std::vector<serialization::converted_value> parameters_;

        void execute_constructor() const;
        void execute_member_function() const;
        void execute_global_function() const;

        std::vector<sol::object> resolve_parameters(sol::state& lua) const;

        bool is_constructor() const noexcept;
        bool is_member_function() const noexcept;
    };
} // namespace engine::pipeline