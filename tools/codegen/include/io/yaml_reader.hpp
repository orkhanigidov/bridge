#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "analysis/analysis_data.hpp"

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
        static YamlReader from_file(const std::filesystem::path& file_path);

        const std::unordered_map<std::string, analysis::ClassConfig>& classes() const noexcept
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
        static constexpr auto TYPES = "types";
        static constexpr auto FREE_FUNCTIONS = "free_functions";

        std::unordered_map<std::string, analysis::ClassConfig> classes_;
        std::vector<std::string> free_functions_;

        void extract_all_classes(const YAML::Node& root);
    };
} // namespace codegen::io
