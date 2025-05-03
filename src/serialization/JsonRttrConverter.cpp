#include "../../include/serialization/JsonRttrConverter.h"

namespace engine::serialization
{

std::optional<rttr::argument> JsonRttrConverter::jsonToArgument(const nlohmann::json &jsonValue,
                                                                const rttr::type &paramType)
{
    try
    {
        if (paramType == rttr::type::get<int>() && jsonValue.is_number_integer())
        {
            return rttr::argument(jsonValue.get<int>());
        }
        if (paramType == rttr::type::get<double>() && jsonValue.is_number())
        {
            return rttr::argument(jsonValue.get<double>());
        }
        if (paramType == rttr::type::get<float>() && jsonValue.is_number())
        {
            return rttr::argument(jsonValue.get<float>());
        }
        if (paramType == rttr::type::get<std::string>() && jsonValue.is_string())
        {
            return rttr::argument(jsonValue.get<std::string>());
        }
        if (paramType == rttr::type::get<bool>() && jsonValue.is_boolean())
        {
            return rttr::argument(jsonValue.get<bool>());
        }

        if (jsonValue.is_null())
        {
            return rttr::argument();
        }

        return std::nullopt;
    }
    catch (const nlohmann::json::exception &)
    {
        return std::nullopt;
    }
}

nlohmann::json JsonRttrConverter::variantToJson(const rttr::variant &result)
{
    if (!result.is_valid())
    {
        return errorToJson("Invalid result variant");
    }

    if (result.is_type<nlohmann::json>())
    {
        return result.get_value<nlohmann::json>();
    }
    if (result.is_type<std::string>())
    {
        return result.get_value<std::string>();
    }
    if (result.is_type<int>())
    {
        return result.get_value<int>();
    }
    if (result.is_type<double>())
    {
        return result.get_value<double>();
    }
    if (result.is_type<float>())
    {
        return result.get_value<float>();
    }
    if (result.is_type<bool>())
    {
        return result.get_value<bool>();
    }

    return errorToJson("Result type not directly convertible to JSON");
}

nlohmann::json JsonRttrConverter::errorToJson(const std::string &message, int code)
{
    return {{"error", true}, {"message", message}, {"code", code}};
}

} // namespace engine::serialization