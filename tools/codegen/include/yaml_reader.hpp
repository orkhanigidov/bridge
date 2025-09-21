#pragma once

#include <yaml-cpp/yaml.h>

namespace codegen {

    class YamlReader final {
    public:
        YamlReader() = default;

        bool load_file(const std::string& filename);

        const std::unordered_map<std::string, std::vector<std::string>>& classes() const noexcept
        {
            return m_classes;
        }

        const std::vector<std::string>& free_functions() const noexcept
        {
            return m_free_functions;
        }

    private:
        static constexpr auto k_name = "name";
        static constexpr auto k_classes = "classes";
        static constexpr auto k_methods = "methods";
        static constexpr auto k_free_functions = "free_functions";

        std::unordered_map<std::string, std::vector<std::string>> m_classes;
        std::vector<std::string> m_free_functions;

        bool extract_classes(const YAML::Node& root);
        std::vector<std::string> extract_methods(const YAML::Node& node);
        bool extract_free_functions(const YAML::Node& root);
    };

} // namespace codegen
