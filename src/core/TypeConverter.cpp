#include "../../include/core/TypeConverter.h"

namespace engine {

    std::pair<bool, rttr::argument> jsonToRttrArgument(const nlohmann::json& jsonValue, const rttr::type& paramType) {
        try {
            if (paramType == rttr::type::get<int>()) {
                return {true, jsonValue.get<int>()};
            }
            if (paramType == rttr::type::get<double>()) {
                return {true, jsonValue.get<double>()};
            }
            if (paramType == rttr::type::get<std::string>()) {
                return {true, jsonValue.get<std::string>()};
            }
            if (paramType == rttr::type::get<bool>()) {
                return {true, jsonValue.get<bool>()};
            }
            return {false, rttr::argument()};
        } catch (const nlohmann::json::exception&) {
            return {false, rttr::argument()};
        }
    }

    nlohmann::json rttrVariantToJson(const rttr::variant& result) {
        if (!result.is_valid()) {
            return createErrorResponse("Method execution failed");
        }

        if (result.is_type<nlohmann::json>()) {
            return result.get_value<nlohmann::json>();
        }
        if (result.is_type<std::string>()) {
            return result.get_value<std::string>();
        }
        if (result.is_type<int>()) {
            return result.get_value<int>();
        }
        if (result.is_type<double>()) {
            return result.get_value<double>();
        }
        if (result.is_type<bool>()) {
            return result.get_value<bool>();
        }

        return {{"result", "Result type not directly convertible to JSON"}};
    }

    nlohmann::json createErrorResponse(const std::string& message) {
        return {{"error", true}, {"message", message}};
    }

} // namespace engine
