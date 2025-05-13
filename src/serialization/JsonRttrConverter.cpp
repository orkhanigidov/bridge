#include "../../include/pch.h"

#include "../../include/serialization/JsonRttrConverter.h"

using namespace rttr;

namespace engine::serialization
{

std::optional<variant> JsonRttrConverter::jsonToVariant(const nlohmann::json &jsonValue, const type &type)
{
    try
    {
        if (jsonValue.is_null())
        {
            return variant();
        }

        if (type == type::get<int>())
        {
            return variant(jsonValue.get<int>());
        }
        if (type == type::get<float>())
        {
            return variant(jsonValue.get<float>());
        }
        if (type == type::get<double>())
        {
            return variant(jsonValue.get<double>());
        }
        if (type == type::get<std::string>())
        {
            return variant(jsonValue.get<std::string>());
        }
        if (type == type::get<bool>())
        {
            return variant(jsonValue.get<bool>());
        }

        return std::nullopt;
    }
    catch (const nlohmann::json::exception &)
    {
        return std::nullopt;
    }
}

std::vector<variant> JsonRttrConverter::convertMethodParams(const method &method, const nlohmann::json &params)
{
    std::vector<variant> args;
    args.reserve(method.get_parameter_infos().size());

    const auto paramList = method.get_parameter_infos();

    for (const auto &param : paramList)
    {
        if (const std::string &paramName = param.get_name().to_string(); params.contains(paramName))
        {
            if (auto arg = jsonToVariant(params.at(paramName), param.get_type()); arg.has_value())
            {
                args.push_back(arg.value());
            }
        }
    }

    return args;
}

nlohmann::json JsonRttrConverter::variantToJson(const variant &returnValue)
{
    if (!returnValue.is_valid())
    {
        return errorToJson("Invalid result variant");
    }

    if (returnValue.is_type<int>())
    {
        return returnValue.get_value<int>();
    }
    if (returnValue.is_type<float>())
    {
        return returnValue.get_value<float>();
    }
    if (returnValue.is_type<double>())
    {
        return returnValue.get_value<double>();
    }
    if (returnValue.is_type<std::string>())
    {
        return returnValue.get_value<std::string>();
    }
    if (returnValue.is_type<bool>())
    {
        return returnValue.get_value<bool>();
    }

    return errorToJson("Result type not directly convertible to JSON");
}

nlohmann::json JsonRttrConverter::errorToJson(const std::string &message, int code)
{
    return {{"error", true}, {"message", message}, {"code", code}};
}

} // namespace engine::serialization