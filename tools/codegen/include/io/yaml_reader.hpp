#pragma once

#include <yaml-cpp/yaml.h>

namespace codegen::io
{
    class YamlReaderException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class YamlReader final
    {
    public:
        static YamlReader from_file(const std::string& filename);

        const std::unordered_map<std::string, std::vector<std::string>>& classes() const noexcept
        {
            return classes_;
        }

        const std::vector<std::string>& free_functions() const noexcept
        {
            return free_functions_;
        }

    private:
        explicit YamlReader(const YAML::Node& root);

        static constexpr auto NAME = "name";
        static constexpr auto CLASSES = "classes";
        static constexpr auto METHODS = "methods";
        static constexpr auto FREE_FUNCTIONS = "free_functions";

        std::unordered_map<std::string, std::vector<std::string>> classes_;
        std::vector<std::string> free_functions_;

        void extract_classes(const YAML::Node& root);
        void extract_free_functions(const YAML::Node& root);
        static std::vector<std::string> extract_methods(const YAML::Node& node);
    };
} // namespace codegen::io
