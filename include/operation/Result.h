#pragma once

#include "../pch.h"

namespace engine::operation
{
    class Result
    {
    public:
        Result() = default;
        explicit Result(const rttr::variant& value);

        const rttr::variant& getValue() const;
        bool hasValue() const;

    private:
        rttr::variant m_value;
        bool m_hasValue{false};
    };
} // namespace engine::operation
