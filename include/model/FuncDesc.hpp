#pragma once

#include "ParamDesc.hpp"
#include "TypeDesc.hpp"

namespace engine::model
{
    class FuncDesc
    {
      public:
        FuncDesc(std::string_view name, const TypeDesc& return_type,
                 std::vector<ParamDesc> parameters, std::string_view category = "",
                 std::string_view description = "")
            : name_(name), return_type_(return_type), parameters_(std::move(parameters)),
              category_(category), description_(description)
        {
        }

        std::string_view name() const noexcept
        {
            return name_;
        }

        const TypeDesc& return_type() const noexcept
        {
            return return_type_;
        }

        const std::vector<ParamDesc>& parameters() const noexcept
        {
            return parameters_;
        }

        std::string_view category() const noexcept
        {
            return category_;
        }

        std::string_view description() const noexcept
        {
            return description_;
        }

        bool has_parameters() const noexcept
        {
            return !parameters_.empty();
        }

      private:
        std::string name_;
        TypeDesc return_type_;
        std::vector<ParamDesc> parameters_;
        std::string category_;
        std::string description_;
    };
} // namespace engine::model