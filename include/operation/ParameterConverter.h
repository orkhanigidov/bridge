#pragma once

#include "../pch.h"
#include "Parameter.h"

namespace engine::operation
{
    Parameter parameterFromJson(const nlohmann::json& parameterValue);
    Parameter parameterFromVariant(const rttr::variant& variant);
} // namespace engine::operation
