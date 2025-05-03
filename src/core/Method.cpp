#include "../../include/core/Method.h"

#include <rttr/registration>

namespace engine {

    void to_json(nlohmann::json& j, const MethodParameter& param) {
        j = {{"name", param.name}, {"type", param.type}, {"defaultValue", param.defaultValue}};
    }

    void from_json(const nlohmann::json& j, MethodParameter& param) {
        param.name         = j.value("name", "");
        param.type         = j.value("type", "");
        param.defaultValue = j.value("defaultValue", "");
    }

    void to_json(nlohmann::json& j, const Method& method) {
        j = {{"name", method.name}, {"returnType", method.returnType}, {"category", method.category},
            {"description", method.description}, {"parameters", method.parameters}};
    }

    void from_json(const nlohmann::json& j, Method& method) {
        method.name        = j.value("name", "");
        method.returnType  = j.value("returnType", "");
        method.category    = j.value("category", "");
        method.description = j.value("description", "");

        method.parameters.clear();
        if (j.contains("parameters") && j["parameters"].is_array()) {
            for (const auto& param : j["parameters"]) {
                method.parameters.push_back(param.get<MethodParameter>());
            }
        }
    }

    RTTR_REGISTRATION {
        rttr::registration::class_<MethodParameter>("MethodParameter")
            .property("name", &MethodParameter::name)
            .property("type", &MethodParameter::type)
            .property("defaultValue", &MethodParameter::defaultValue);

        rttr::registration::class_<Method>("MethodDescriptor")
            .property("name", &Method::name)
            .property("returnType", &Method::returnType)
            .property("parameters", &Method::parameters)
            .property("category", &Method::category)
            .property("description", &Method::description);
    }

} // namespace engine
