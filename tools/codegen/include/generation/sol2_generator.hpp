/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
    class Sol2GeneratorException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class Sol2Generator final
    {
    public:
        explicit Sol2Generator(const std::filesystem::path& output_file): output_file_(std::move(output_file))
        {
        }

        void generate(const std::unordered_set<std::string>& includes,
                      const std::unordered_set<std::string>& namespaces,
                      const std::vector<metadata::ClassDescriptor>& classes,
                      const std::vector<metadata::FunctionDescriptor>& free_functions,
                      const std::vector<metadata::EnumDescriptor>& enums) const;

    private:
        std::filesystem::path output_file_;

        static void write_header(std::ofstream& out,
                                 const std::unordered_set<std::string>& includes,
                                 const std::unordered_set<std::string>& namespaces,
                                 const std::unordered_set<std::string>& container_types);
        static void write_member_registrations(std::ofstream& out, const std::vector<metadata::ClassDescriptor>& classes);
        static void write_non_member_registrations(std::ofstream& out,
                                                   const std::vector<metadata::FunctionDescriptor>& free_functions,
                                                   const std::vector<metadata::EnumDescriptor>& enums);
        static void write_footer(std::ofstream& out);
    };
} // namespace codegen::generation
