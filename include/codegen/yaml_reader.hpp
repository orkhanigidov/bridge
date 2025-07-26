#pragma once

#include <yaml-cpp/yaml.h>

namespace codegen {
    class YamlReader final {
    public:
        YamlReader() = default;

        bool load_file(const std::string& filename);

        [[nodiscard]] const std::unordered_map<std::string, std::vector<std::string>>& classes() const noexcept { return m_classes; }

        [[nodiscard]] const std::vector<std::string>& free_functions() const noexcept { return m_free_functions; }

    private:
        static constexpr auto kName = "name";
        static constexpr auto kClasses = "classes";
        static constexpr auto kMethods = "methods";
        static constexpr auto kFreeFunctions = "free_functions";

        std::unordered_map<std::string, std::vector<std::string>> m_classes;
        std::vector<std::string> m_free_functions;

        bool extract_classes(const YAML::Node& root);
        std::vector<std::string> extract_methods(const YAML::Node& node);
        bool extract_free_functions(const YAML::Node& root);
    };
} // namespace codegen
