#include "../../include/pch.h"

#include "../../include/serialization/MethodJsonSerializer.h"

using json = nlohmann::json;

namespace engine::serialization
{

void to_json(nlohmann::json &j, const model::MethodParameterDescriptor &param)
{
    j = json{{"name", param.name}, {"type", param.type}, {"defaultValue", param.defaultValue}};
}

void from_json(const nlohmann::json &j, model::MethodParameterDescriptor &param)
{
    j.at("name").get_to(param.name);
    j.at("type").get_to(param.type);
    j.at("defaultValue").get_to(param.defaultValue);
}

void to_json(nlohmann::json &j, const model::MethodDescriptor &method)
{
    j = json{{"name", method.name},
             {"returnType", method.returnType},
             {"category", method.category},
             {"description", method.description}};

    j["parameters"] = json::array();
    for (const auto &param : method.parameters)
    {
        json param_json;
        to_json(param_json, param);
        j["parameters"].push_back(param_json);
    }
}

void from_json(const nlohmann::json &j, model::MethodDescriptor &method)
{
    j.at("name").get_to(method.name);
    j.at("returnType").get_to(method.returnType);
    j.at("category").get_to(method.category);
    j.at("description").get_to(method.description);

    method.parameters.clear();
    for (const auto &param_json : j.at("parameters"))
    {
        model::MethodParameterDescriptor param;
        from_json(param_json, param);
        method.parameters.push_back(param);
    }
}

} // namespace engine::serialization