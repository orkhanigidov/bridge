#pragma once

#include "metadata/class_descriptor.hpp"

namespace codegen::generation {

    using class_descriptor = engine::metadata::ClassDescriptor;
    using function_descriptor = engine::metadata::FunctionDescriptor;

    class Sol2Generator final {
    public:
        explicit Sol2Generator(const fs::path& output_file)
            : output_file_(output_file) {}

        void generate(const std::vector<std::string>& includes,
                      const std::vector<class_descriptor>& classes,
                      const std::vector<function_descriptor>& free_functions);

    private:
        fs::path output_file_;

        void write_class_registrations(const std::vector<class_descriptor>& classes, std::ofstream& out);
        void write_free_function_registrations(const std::vector<function_descriptor>& free_functions, std::ofstream& out);
        void write_line(std::ofstream& out, int indent_level, const std::string& text, int newlines = 1);
    };

} // namespace codegen::generation
