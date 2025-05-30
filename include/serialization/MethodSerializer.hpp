#pragma once

#include "model/Method.hpp"
#include "pch.hpp"

namespace engine::serialization
{
    class MethodSerializer final
    {
      public:
        MethodSerializer() = delete;

        [[nodiscard]] static nlohmann::json toJson(const model::Method& method);
        [[nodiscard]] static std::optional<model::Method> fromJson(const nlohmann::json& json);

      private:
        [[nodiscard]] static nlohmann::json parameterToJson(const model::Parameter& parameter);
        [[nodiscard]] static std::optional<model::Parameter>
        parameterFromJson(const nlohmann::json& json);

        template <typename T>
        [[nodiscard]] static bool isValidJsonField(const nlohmann::json& json,
                                                   std::string_view fieldName)
        {
            if (!json.contains(fieldName) || json[fieldName].is_null())
                return false;

            try
            {
                json[fieldName].get<T>();
                return true;
            }
            catch (const nlohmann::json::exception&)
            {
                return false;
            }
        }

        struct JsonFields
        {
            // Method fields
            static constexpr std::string_view METHOD_NAME = "method_name";
            static constexpr std::string_view RETURN_TYPE = "return_type";
            static constexpr std::string_view CATEGORY    = "category";
            static constexpr std::string_view DESCRIPTION = "description";
            static constexpr std::string_view PARAMETERS  = "parameters";
            static constexpr std::string_view IS_STATIC   = "is_static";

            // Parameter fields
            static constexpr std::string_view PARAM_NAME    = "param_name";
            static constexpr std::string_view PARAM_TYPE    = "param_type";
            static constexpr std::string_view DEFAULT_VALUE = "default_value";
        };
    };
} // namespace engine::serialization