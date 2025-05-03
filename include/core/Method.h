#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace engine {

    struct MethodParameter final {
        std::string name;
        std::string type;
        std::string defaultValue;
    };

    struct Method final {
        std::string name;
        std::string returnType;
        std::vector<MethodParameter> parameters;
        std::string category;
        std::string description;
    };

    void to_json(nlohmann::json& j, const MethodParameter& param);
    void from_json(const nlohmann::json& j, MethodParameter& param);

    void to_json(nlohmann::json& j, const Method& method);
    void from_json(const nlohmann::json& j, Method& method);

} // namespace engine
