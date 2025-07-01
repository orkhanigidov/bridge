#pragma once

#include "FuncDesc.hpp"
#include "pch.hpp"

namespace engine::model
{
    class ClassDesc
    {
      public:
        ClassDesc(std::string_view name, std::string_view alias) : name_(name), alias_(alias) {}

        std::string_view name() const noexcept
        {
            return name_;
        }

        std::string_view alias() const noexcept
        {
            return alias_;
        }

        const std::vector<FuncDesc>& functions() const noexcept
        {
            return functions_;
        }

        void add_function(FuncDesc func)
        {
            functions_.push_back(std::move(func));
        }

        bool has_functions() const noexcept
        {
            return !functions_.empty();
        }

      private:
        std::string name_;
        std::string alias_;
        std::vector<FuncDesc> functions_;
    };
} // namespace engine::model