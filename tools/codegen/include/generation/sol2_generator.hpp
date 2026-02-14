/**
 * Project: Engine
 * Tool: Codegen
 * File: sol2_generator.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file sol2_generator.hpp
 * @brief Declares the Sol2Generator class for generating Lua bindings using Sol2.
 */

#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "metadata/class_descriptor.hpp"
#include "metadata/enum_descriptor.hpp"
#include "metadata/function_descriptor.hpp"

namespace codegen::generation
{
    /**
     * @class Sol2GeneratorException
     * @brief Exception type for Sol2Generator errors.
     */
    class Sol2GeneratorException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    /**
     * @class Sol2Generator
     * @brief Generates Lua bindings using Sol2 for C++ classes, functions, and enums.
     *
     * This class writes C++ code that registers classes, free functions, and enums with the Sol2 Lua binding library. Output is written to the specified file.
     */
    class Sol2Generator final
    {
    public:
        /**
         * @brief Constructs a Sol2Generator with the given output file path.
         * @param output_file Path to the file where generated code will be written.
         */
        explicit Sol2Generator(const std::filesystem::path& output_file) : output_file_(std::move(output_file))
        {
        }

        /**
         * @brief Generates Sol2 bindings for the provided metadata.
         * @param includes Set of header files to include.
         * @param namespaces Set of C++ namespaces to process.
         * @param containers Set of containers to specialize.
         * @param classes List of class descriptors to generate bindings for.
         * @param free_functions List of free function descriptors to bind.
         * @param enums List of enum descriptors to bind.
         * @param manual_bindings List of (header_path, function_name) for manual bindings.
         */
        void generate(const std::unordered_set<std::string>& includes,
                      const std::unordered_set<std::string>& namespaces,
                      const std::unordered_set<std::string>& containers,
                      const std::vector<metadata::ClassDescriptor>& classes,
                      const std::vector<metadata::FunctionDescriptor>& free_functions,
                      const std::vector<metadata::EnumDescriptor>& enums,
                      const std::vector<std::pair<std::string, std::string>>& manual_bindings) const;

    private:
        std::filesystem::path output_file_;

        /**
         * @brief Writes the file header, including and namespace declarations.
         * @param out Output file stream.
         * @param includes Set of header files to include.
         * @param namespaces Set of namespaces to open.
         * @param containers Set of containers to specialize.
         * @param classes List of class descriptors.
         * @param manual_bindings List of manual binding headers to include.
         */
        static void write_header(std::ofstream& out,
                                 const std::unordered_set<std::string>& includes,
                                 const std::unordered_set<std::string>& namespaces,
                                 const std::unordered_set<std::string>& containers,
                                 const std::vector<metadata::ClassDescriptor>& classes,
                                 const std::vector<std::pair<std::string, std::string>>& manual_bindings);

        /**
         * @brief Writes Sol2 member registrations for all classes.
         * @param out Output file stream.
         * @param classes List of class descriptors.
         */
        static void write_member_registrations(std::ofstream& out, const std::vector<metadata::ClassDescriptor>& classes);

        /**
         * @brief Writes Sol2 registrations for free functions and enums.
         * @param out Output file stream.
         * @param free_functions List of free function descriptors.
         * @param enums List of enum descriptors.
         */
        static void write_non_member_registrations(std::ofstream& out,
                                                   const std::vector<metadata::FunctionDescriptor>& free_functions,
                                                   const std::vector<metadata::EnumDescriptor>& enums);

        /**
         * @brief Writes calls to manual registration functions.
         * @param out Output file stream.
         * @param manual_bindings List of (header_path, function_name) for manual bindings.
         */
        static void write_manual_bindings(std::ofstream& out, const std::vector<std::pair<std::string, std::string>>& manual_bindings);

        /**
         * @brief Writes the file footer, closing any open namespaces.
         * @param out Output file stream.
         */
        static void write_footer(std::ofstream& out);
    };
} // namespace codegen::generation
