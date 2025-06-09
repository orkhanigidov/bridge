#pragma once

#include "pch.hpp"

namespace engine::pipeline
{
    class Command final
    {
      public:
        explicit Command(const nlohmann::json& json);
        ~Command() = default;

        std::string_view name() const noexcept
        {
            return name_;
        }

        std::optional<std::string_view> alias() const noexcept
        {
            return alias_;
        }

        void execute();

      private:
        std::string name_;
        std::optional<std::string> alias_;
        std::unordered_map<std::string, nlohmann::json> parameters_;

        void execute_constructor();
        void execute_method();
        void execute_global_method();

        bool is_constructor() const noexcept;
        bool is_method_call() const noexcept;

        std::vector<rttr::variant> resolve_arguments(const rttr::method& method);
        std::vector<rttr::variant> resolve_arguments_for_constructor(const rttr::constructor& ctor);
    };
} // namespace engine::pipeline