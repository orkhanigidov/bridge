#pragma once

#include "metadata/class_descriptor.hpp"
#include "metadata/enum_descriptor.hpp"

namespace codegen::generation
{
    class Sol2Generator final
    {
    public:
        explicit Sol2Generator(const fs::path& output_file): output_file_(output_file)
        {
        }

        void generate(const std::unordered_set<std::string>& includes,
                      const std::vector<metadata::ClassDescriptor>& classes,
                      const std::vector<metadata::FunctionDescriptor>& free_functions,
                      const std::vector<metadata::EnumDescriptor>& enums) const;

    private:
        fs::path output_file_;

        static void write_header(std::ofstream& out, const std::unordered_set<std::string>& includes);
        static void write_member_registrations(std::ofstream& out, const std::vector<metadata::ClassDescriptor>& classes);
        static void write_non_member_registrations(std::ofstream& out,
                                                   const std::vector<metadata::FunctionDescriptor>& free_functions,
                                                   const std::vector<metadata::EnumDescriptor>& enums);
        static void write_footer(std::ofstream& out);
    };
} // namespace codegen::generation
