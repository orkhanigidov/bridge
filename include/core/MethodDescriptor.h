#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace engine {

struct MethodParameter final {
  std::string name;
  std::string type;
  std::string defaultValue;
};

struct MethodDescriptor final {
  std::string name;
  std::string returnType;
  std::vector<MethodParameter> parameters;
  std::string category;
  std::string description;
};

void to_json(nlohmann::json &j, const MethodDescriptor &method);
void from_json(const nlohmann::json &j, MethodDescriptor &method);

} // namespace engine