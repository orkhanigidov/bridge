#include "../../include/pch.h"

#include "../../include/model/Method.h"

namespace engine::model
{
    Method::Method(const rttr::method& method, const rttr::type& return_type, std::vector<Parameter> parameters,
                   bool is_static, std::string_view category, std::string_view description)
        : method_(method), return_type_(return_type), parameters_(std::move(parameters)), is_static_(is_static),
          category_(category), description_(description)
    {
        if (!method.is_valid())
            throw std::invalid_argument("Invalid method provided");

        if (!return_type.is_valid())
            throw std::invalid_argument("Invalid return type provided");
    }
} // namespace engine::model