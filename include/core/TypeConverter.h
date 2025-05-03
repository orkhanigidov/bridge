#pragma once

#include <rttr/variant.h>

#include <nlohmann/json.hpp>

namespace engine {

    std::pair<bool, rttr::argument> jsonToRttrArgument(const nlohmann::json& jsonValue, const rttr::type& paramType);

    nlohmann::json rttrVariantToJson(const rttr::variant& result);

    nlohmann::json createErrorResponse(const std::string& message);

} // namespace engine
