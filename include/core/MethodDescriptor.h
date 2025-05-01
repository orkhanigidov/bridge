#pragma once

#include <nlohmann/json.hpp>
#include <rttr/registration>
#include <string>
#include <vector>

namespace engine {

struct MethodParameter final {
protected:
  ~MethodParameter() = default;

public:
  std::string name;
  std::string type;
  std::string defaultValue;

  RTTR_ENABLE()
};

struct MethodDescriptor final {
protected:
  ~MethodDescriptor() = default;

public:
  std::string name;
  std::string returnType;
  std::vector<MethodParameter> parameters;
  std::string category;
  std::string description;

  RTTR_ENABLE()
};

void to_json(nlohmann::json &j, const MethodDescriptor &method);
void from_json(const nlohmann::json &j, MethodDescriptor &method);

} // namespace engine