#include "../../include/pch.h"

#include "../../include/model/Class.h"

namespace engine::model
{
    Class::Class(std::string_view id, const rttr::type& type) : id_(id), type_(type)
    {
        if (id.empty())
            throw std::invalid_argument("Class ID cannot be empty");

        if (!type.is_valid())
            throw std::invalid_argument("Invalid RTTR type provided for Class");
    }
} // namespace engine::model