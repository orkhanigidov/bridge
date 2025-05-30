#include "serialization/RttrConverter.hpp"

#include "model/Method.hpp"
#include "pch.hpp"
#include "pipeline/ObjectPool.hpp"

#include <string>
#include <string_view>

namespace engine::serialization
{
    std::vector<rttr::variant> RttrConverter::prepareMethodArguments(const model::Method& method,
                                                                     const nlohmann::json& json)
    {
        if (!json.is_object())
            throw std::invalid_argument(
                "JSON input must be an object for method argument preparation");

        const std::vector<model::Parameter>& parameters = method.parameters();
        std::vector<rttr::variant> args;
        args.reserve(parameters.size());

        for (const auto& parameter : parameters)
        {
            if (std::string_view parameterName = parameter.name();
                json.contains(std::string(parameterName)))
            {
                const std::optional<rttr::variant> arg = fromJson(json, parameter.type());

                if (arg.has_value())
                    args.emplace_back(arg.value());
                else
                    throw std::runtime_error("Failed to convert JSON to variant for parameter: " +
                                             std::string(parameterName));
            }
        }

        return args;
    }

    std::optional<rttr::variant> RttrConverter::fromJson(const nlohmann::json& json,
                                                         const rttr::type& type)
    {
        if (!type.is_valid())
            return std::nullopt;

        try
        {
            if (json.is_object() && json.contains("ref"))
                return pipeline::ObjectPool::instance().get_object(json["ref"].get<std::string>());

            if (json.is_number_integer() && type == rttr::type::get<int>())
                return json.get<int>();
            if (json.is_string() && type == rttr::type::get<std::string>())
                return json.get<std::string>();
            if (json.is_boolean() && type == rttr::type::get<bool>())
                return json.get<bool>();

            if (json.is_number_float())
            {
                if (type == rttr::type::get<float>())
                    return json.get<float>();
                if (type == rttr::type::get<double>())
                    return json.get<double>();
            }

            if (json.is_array())
            {
            }

            if (json.is_object())
            {
            }
        }
        catch (const std::exception&)
        {
            return std::nullopt;
        }

        return std::nullopt;
    }

    nlohmann::json RttrConverter::toJson(const rttr::variant& variant)
    {
        if (!variant.is_valid())
            throw std::invalid_argument("Invalid variant provided for JSON conversion");

        if (variant == rttr::type::get<int>())
            return variant.get_value<int>();
        if (variant == rttr::type::get<float>())
            return variant.get_value<float>();
        if (variant == rttr::type::get<double>())
            return variant.get_value<double>();
        if (variant == rttr::type::get<std::string>())
            return variant.get_value<std::string>();
        if (variant == rttr::type::get<bool>())
            return variant.get_value<bool>();

        if (variant.is_sequential_container())
        {
        }

        if (variant.get_type().is_class())
        {
        }

        return {};
    }
} // namespace engine::serialization