#pragma once

#include "function_descriptor.hpp"
#include "pch.hpp"

namespace engine::metadata
{
    class class_descriptor final
    {
      public:
        class_descriptor(std::string_view name, std::string_view alias) : name_(name), alias_(alias)
        {
        }

        std::string_view name() const noexcept
        {
            return name_;
        }

        std::string_view alias() const noexcept
        {
            return alias_;
        }

        const std::vector<function_descriptor>& functions() const noexcept
        {
            return functions_;
        }

        void add_function(function_descriptor function)
        {
            functions_.emplace_back(std::move(function));
        }

        bool has_functions() const noexcept
        {
            return !functions_.empty();
        }

      private:
        std::string name_;
        std::string alias_;
        std::vector<function_descriptor> functions_;
    };
} // namespace engine::metadata