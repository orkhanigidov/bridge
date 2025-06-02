#include "serialization/MethodSerializer.hpp"

#include "model/Method.hpp"
#include "pch.hpp"
#include "reflection/ReflectionRegistry.hpp"
#include "serialization/RttrConverter.hpp"

#include <string>

namespace engine::serialization
{
    nlohmann::json MethodSerializer::parameterToJson(const model::Parameter& parameter)
    {
        nlohmann::json result;

        result[JsonFields::PARAM_NAME] = parameter.name();
        result[JsonFields::PARAM_TYPE] = parameter.type().get_name().to_string();

        const rttr::variant& defaultValue = parameter.default_value();
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
            if (isValidJsonField<std::string>(json, JsonFields::DEFAULT_VALUE))
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

        result[JsonFields::METHOD_NAME] = method.method().get_name().to_string();
        result[JsonFields::RETURN_TYPE] = method.return_type().get_name().to_string();
        result[JsonFields::CATEGORY]    = method.category();
        result[JsonFields::DESCRIPTION] = method.description();
        result[JsonFields::IS_STATIC]   = method.is_static();

        result[JsonFields::PARAMETERS] = nlohmann::json::array();
        for (const auto& parameter : method.parameters())
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
            const std::string name = json[JsonFields::METHOD_NAME].get<std::string>();
            const model::Method* methodPtr =
                reflection::ReflectionRegistry::instance().get_method(name);

            if (!methodPtr || !methodPtr->method().is_valid())
                return std::nullopt;

            const rttr::method method = methodPtr->method();

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

            return model::Method(method, returnType, std::move(parameters), isStatic, category,
                                 std::move(description));
        }
        catch (const std::exception&)
        {
            return std::nullopt;
        }
    }
} // namespace engine::serialization