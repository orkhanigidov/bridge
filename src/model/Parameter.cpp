#include "../../include/pch.h"

#include "../../include/model/Parameter.h"

namespace engine::model
{
    Parameter::Parameter(std::string_view name, const rttr::type& type, rttr::variant default_value, bool is_reference)
        : name_(name), type_(type), default_value_(std::move(default_value)), is_reference_(is_reference)
    {
        if (name.empty())
            throw std::invalid_argument("Parameter name cannot be empty");

        if (!type.is_valid())
            throw std::invalid_argument("Invalid type provided for parameter");
    }
} // namespace engine::model