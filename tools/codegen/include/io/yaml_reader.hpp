/**
 * Project: Engine
 * Tool: Codegen
 * File: yaml_reader.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file yaml_reader.hpp
 * @brief Declares the YamlReader for reading and parsing YAML configuration files.
 */

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
    /**
     * @class YamlReaderException
     * @brief Exception thrown by YamlReader on errors.
     */
    class YamlReaderException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    /**
     * @class YamlReader
     * @brief Reads and parses YAML configuration files for code generation.
     *
     * Provides access to parsed class configurations, free functions, and enums from a YAML file.
     */
    class YamlReader final
    {
    public:
        /**
         * @brief Loads and parses a YAML file into a YamlReader.
         * @param file_path Path to the YAML configuration file.
         * @return YamlReader instance with parsed data.
         */
        static YamlReader from_file(const std::filesystem::path& file_path);

        /**
         * @brief Gets the parsed class configurations.
         * @return Map of class names to ClassConfig.
         */
        const std::unordered_map<std::string, analysis::ClassConfig>& classes() const noexcept
        {
            return classes_;
        }

        /**
         * @brief Gets the list of free function names.
         * @return Vector of free function names.
         */
        const std::vector<std::string>& free_functions() const noexcept
        {
            return free_functions_;
        }

        /**
         * @brief Gets the list of enum names.
         * @return Vector of enum names.
         */
        const std::vector<std::string>& enums() const noexcept
        {
            return enums_;
        }

    private:
        /**
         * @brief Constructs a YamlReader from a parsed YAML node.
         * @param root The root YAML node.
         */
        explicit YamlReader(const YAML::Node& root);

        static constexpr auto NAME = "name";
        static constexpr auto CLASSES = "classes";
        static constexpr auto METHODS = "methods";
        static constexpr auto TYPES = "types";
        static constexpr auto FREE_FUNCTIONS = "free_functions";
        static constexpr auto ENUMS = "enums";

        /**
         * @brief Parsed class configurations.
         */
        std::unordered_map<std::string, analysis::ClassConfig> classes_;

        /**
         * @brief List of free function names.
         */
        std::vector<std::string> free_functions_;

        /**
         * @brief List of enum names.
         */
        std::vector<std::string> enums_;

        /**
         * @brief Extracts all class configurations from the YAML root node.
         * @param root The root YAML node.
         */
        void extract_all_classes(const YAML::Node& root);
    };
} // namespace codegen::io
