#include "../../include/core/MethodDescriptor.h"

namespace engine {

void to_json(nlohmann::json &j, const MethodParameter &param) {
  j = nlohmann::json::object();
  j["name"] = param.name;
  j["type"] = param.type;
  j["defaultValue"] = param.defaultValue;
}

void from_json(const nlohmann::json &j, MethodParameter &param) {
  j.at("name").get_to(param.name);
  j.at("type").get_to(param.type);
  j.at("defaultValue").get_to(param.defaultValue);
}

void to_json(nlohmann::json &j, const MethodDescriptor &method) {
  j = nlohmann::json::object();
  j["name"] = method.name;
  j["returnType"] = method.returnType;
  j["category"] = method.category;
  j["description"] = method.description;
  j["parameters"] = method.parameters;
}

void from_json(const nlohmann::json &j, MethodDescriptor &method) {
  j.at("name").get_to(method.name);
  j.at("returnType").get_to(method.returnType);
  j.at("category").get_to(method.category);
  j.at("description").get_to(method.description);

  method.parameters.clear();
  for (const auto &param : j.at("parameters")) {
    method.parameters.push_back(param.get<MethodParameter>());
  }
}

RTTR_REGISTRATION {
  rttr::registration::class_<MethodParameter>("MethodParameter")
      .property("name", &MethodParameter::name)
      .property("type", &MethodParameter::type)
      .property("defaultValue", &MethodParameter::defaultValue);

  rttr::registration::class_<MethodDescriptor>("MethodDescriptor")
      .property("name", &MethodDescriptor::name)
      .property("returnType", &MethodDescriptor::returnType)
      .property("parameters", &MethodDescriptor::parameters)
      .property("category", &MethodDescriptor::category)
      .property("description", &MethodDescriptor::description);
}

} // namespace engine