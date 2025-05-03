#pragma once

#include "../model/MethodDescriptor.h"
#include <nlohmann/json.hpp>

namespace engine::serialization
{

void to_json(nlohmann::json &j, const model::MethodParameterDescriptor &param);
void from_json(const nlohmann::json &j, model::MethodParameterDescriptor &param);

void to_json(nlohmann::json &j, const model::MethodDescriptor &method);
void from_json(const nlohmann::json &j, model::MethodDescriptor &method);

} // namespace engine::serialization