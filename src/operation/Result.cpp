#include "../../include/pch.h"

#include "../../include/operation/Result.h"
#include "../../include/serialization/RttrConverter.h"

namespace engine::operation
{
    Result::Result(rttr::variant value) : value_(std::move(value))
    {
        if (!value.is_valid())
            throw std::invalid_argument("Invalid value provided to Result");

        if (value.is_type<void>())
            value_ = rttr::variant();

        value_ = serialization::RttrConverter::to_json(value);
    }

    bool Result::has_value() const noexcept
    {
        return value_.is_valid();
    }

    const rttr::variant& Result::value() const noexcept
    {
        if (!has_value())
            throw std::runtime_error("Result does not have a value");

        return value_;
    }
} // namespace engine::operation