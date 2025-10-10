#pragma once

#include <yaml-cpp/yaml.h>

namespace codegen::io {

    class YamlReader final {
    public:
        YamlReader() = default;

        bool load_file(const std::string& filename);

        const std::unordered_map<std::string, std::vector<std::string>>& classes() const noexcept
        {
            return classes_;
        }

        const std::vector<std::string>& free_functions() const noexcept
        {
            return free_functions_;
        }

    private:
        static constexpr auto k_name = "name";
        static constexpr auto k_classes = "classes";
        static constexpr auto k_methods = "methods";
        static constexpr auto k_free_functions = "free_functions";

        std::unordered_map<std::string, std::vector<std::string>> classes_;
        std::vector<std::string> free_functions_;

        bool extract_classes(const YAML::Node& root);
        std::vector<std::string> extract_methods(const YAML::Node& node);
        bool extract_free_functions(const YAML::Node& root);
    };

} // namespace codegen::io
