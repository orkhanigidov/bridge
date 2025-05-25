#include "../../include/serialization/MethodSerializer.h"

#include "../../include/pch.h"
#include "../../include/reflection/ReflectionRegistry.h"
#include "../../include/serialization/RttrConverter.h"

namespace engine::serialization
{
    nlohmann::json MethodSerializer::parameterToJson(const model::Parameter& parameter)
    {
        nlohmann::json result;

        result[JsonFields::PARAM_NAME] = parameter.getName();
        result[JsonFields::PARAM_TYPE] = parameter.getType().get_name().to_string();

        const rttr::variant& defaultValue = parameter.getDefaultValue();
        if (defaultValue.is_valid())
            result[JsonFields::DEFAULT_VALUE] = RttrConverter::toJson(defaultValue);
        else
            result[JsonFields::DEFAULT_VALUE] = nullptr;

        return result;
    }

    std::optional<model::Parameter> MethodSerializer::parameterFromJson(const nlohmann::json& json)
    {
        if (!isValidJsonField<std::string>(json, JsonFields::PARAM_NAME) ||
            !isValidJsonField<std::string>(json, JsonFields::PARAM_TYPE))
            return std::nullopt;

        try
        {
            const std::string name    = json[JsonFields::PARAM_NAME].get<std::string>();
            const std::string typeStr = json[JsonFields::PARAM_TYPE].get<std::string>();

            if (name.empty() || typeStr.empty())
                return std::nullopt;

            const rttr::type type = rttr::type::get_by_name(typeStr);
            if (!type.is_valid())
                return std::nullopt;

            rttr::variant defaultValue;
            if (isValidJsonField<std::any>(json, JsonFields::DEFAULT_VALUE))
                defaultValue = RttrConverter::fromJson(json[JsonFields::DEFAULT_VALUE], type);

            return model::Parameter(name, type, std::move(defaultValue));
        }
        catch (const std::exception&)
        {
            return std::nullopt;
        }
    }

    nlohmann::json MethodSerializer::toJson(const model::Method& method)
    {
        nlohmann::json result;

        result[JsonFields::METHOD_NAME] = method.getMethod().get_name().to_string();
        result[JsonFields::RETURN_TYPE] = method.getReturnType().get_name().to_string();
        result[JsonFields::CATEGORY]    = method.getCategory();
        result[JsonFields::DESCRIPTION] = method.getDescription();
        result[JsonFields::IS_STATIC]   = method.isStatic();

        result[JsonFields::PARAMETERS] = nlohmann::json::array();
        for (const auto& parameter : method.getParameters())
        {
            result[JsonFields::PARAMETERS].emplace_back(parameterToJson(parameter));
        }

        return result;
    }

    std::optional<model::Method> MethodSerializer::fromJson(const nlohmann::json& json)
    {
        if (!isValidJsonField<std::string>(json, JsonFields::METHOD_NAME) ||
            !isValidJsonField<std::string>(json, JsonFields::RETURN_TYPE) ||
            !isValidJsonField<std::string>(json, JsonFields::CATEGORY) ||
            !isValidJsonField<nlohmann::json::array_t>(json, JsonFields::PARAMETERS))
            return std::nullopt;

        try
        {
            const std::string name         = json[JsonFields::METHOD_NAME].get<std::string>();
            const model::Method* methodPtr = reflection::ReflectionRegistry::getInstance().getMethod(name);

            if (!methodPtr || !methodPtr->getMethod().is_valid())
                return std::nullopt;

            const rttr::method method = methodPtr->getMethod();

            const std::string returnTypeStr = json[JsonFields::RETURN_TYPE].get<std::string>();
            const rttr::type returnType     = rttr::type::get_by_name(returnTypeStr);

            if (!returnType.is_valid())
                return std::nullopt;

            std::vector<model::Parameter> parameters;
            parameters.reserve(json[JsonFields::PARAMETERS].size());
            for (const auto& parameter : json[JsonFields::PARAMETERS])
            {
                auto result = parameterFromJson(parameter);
                if (!result.has_value())
                    return std::nullopt;

                parameters.emplace_back(std::move(*result));
            }

            const std::string category = json[JsonFields::CATEGORY].get<std::string>();

            if (category.empty())
                return std::nullopt;

            std::string description;
            if (isValidJsonField<std::string>(json, JsonFields::DESCRIPTION))
                description = json[JsonFields::DESCRIPTION].get<std::string>();

            bool isStatic = true;
            if (isValidJsonField<bool>(json, JsonFields::IS_STATIC))
                isStatic = json[JsonFields::IS_STATIC].get<bool>();

            return model::Method(method, returnType, std::move(parameters), category, std::move(description), isStatic);
        }
        catch (const std::exception&)
        {
            return std::nullopt;
        }
    }
} // namespace engine::serialization