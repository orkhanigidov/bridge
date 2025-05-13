#pragma once

#include "../pch.h"

#include "../model/MethodDescriptor.h"

namespace engine::serialization
{

class JsonSerializer
{
  public:
    static nlohmann::json serialize(const model::MethodDescriptor &method);
    static std::optional<model::MethodDescriptor> deserialize(const nlohmann::json &j);

  private:
    static nlohmann::json serializeParameter(const model::MethodParameterDescriptor &param);
    static std::optional<model::MethodParameterDescriptor> deserializeParameter(const nlohmann::json &j);
};

} // namespace engine::serialization