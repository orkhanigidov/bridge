#include "../../include/pch.h"

#include "../../include/serialization/JsonSerializer.h"

#include "../../include/serialization/JsonRttrConverter.h"

using json = nlohmann::json;

namespace engine::serialization
{

nlohmann::json JsonSerializer::serializeParameter(const model::MethodParameterDescriptor &param)
{
    nlohmann::json j;
    j["name"] = param.getName();
    j["paramType"] = param.getParamType().get_name().to_string();

    if (param.getDefaultValue().is_valid())
    {
        j["defaultValue"] = JsonRttrConverter::variantToJson(param.getDefaultValue());
    }
    else
    {
        j["defaultValue"] = nullptr;
    }

    return j;
}

std::optional<model::MethodParameterDescriptor> JsonSerializer::deserializeParameter(const nlohmann::json &j)
{
    try
    {
        if (!j.contains("name") || !j.contains("paramType"))
        {
            return std::nullopt;
        }

        auto nameStr = j["name"].get<std::string>();
        const rttr::argument name(nameStr);

        const rttr::type paramType = rttr::type::get_by_name(j["paramType"].get<std::string>());
        if (!paramType.is_valid())
        {
            return std::nullopt;
        }

        rttr::variant defaultValue;
        if (j.contains("defaultValue") && !j["defaultValue"].is_null())
        {
            if (const auto variantOpt = JsonRttrConverter::jsonToVariant(j["defaultValue"], paramType))
            {
                defaultValue = *variantOpt;
            }
        }

        return model::MethodParameterDescriptor(name, paramType, defaultValue);
    }
    catch (const std::exception &)
    {
        return std::nullopt;
    }
}

nlohmann::json JsonSerializer::serialize(const model::MethodDescriptor &method)
{
    nlohmann::json j;
    j["method"] = method.getMethod().get_name();
    j["returnType"] = method.getReturnType().get_name();
    j["category"] = method.getCategory();
    j["description"] = method.getDescription();

    j["parameters"] = nlohmann::json::array();
    for (const auto &param : method.getParameters())
    {
        j["parameters"].push_back(serializeParameter(param));
    }

    return j;
}

std::optional<model::MethodDescriptor> JsonSerializer::deserialize(const nlohmann::json &j)
{
    try
    {
        if (!j.contains("method") || !j.contains("returnType") || !j.contains("parameters") || !j.contains("category"))
        {
            return std::nullopt;
        }

        const rttr::method method = rttr::type::get_global_method(j["method"].get<std::string>());
        if (!method.is_valid())
        {
            return std::nullopt;
        }

        const rttr::type returnType = rttr::type::get_by_name(j["returnType"].get<std::string>());
        const std::string category = j["category"].get<std::string>();
        const std::string description = j["description"].get<std::string>();

        std::vector<model::MethodParameterDescriptor> parameters;
        for (const auto &paramJson : j["parameters"])
        {
            auto paramOpt = deserializeParameter(paramJson);
            if (!paramOpt)
            {
                return std::nullopt;
            }

            parameters.push_back(*paramOpt);
        }

        return model::MethodDescriptor(method, returnType, parameters, category, description);
    }
    catch (const std::exception &)
    {
        return std::nullopt;
    }
}

} // namespace engine::serialization