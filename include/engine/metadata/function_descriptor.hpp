#pragma once

#include "parameter_descriptor.hpp"
#include "type_descriptor.hpp"

namespace engine::metadata
{
    class function_descriptor final
    {
      public:
        function_descriptor(std::string_view name, type_descriptor return_type,
                            std::vector<parameter_descriptor> parameters,
                            std::string_view category = "", std::string_view description = "")
            : name_(name), return_type_(std::move(return_type)), parameters_(std::move(parameters)),
              category_(category), description_(description)
        {
        }

        std::string_view name() const noexcept
        {
            return name_;
        }

        const type_descriptor& return_type() const noexcept
        {
            return return_type_;
        }

        const std::vector<parameter_descriptor>& parameters() const noexcept
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
        type_descriptor return_type_;
        std::vector<parameter_descriptor> parameters_;
        std::string category_;
        std::string description_;
    };
} // namespace engine::metadata