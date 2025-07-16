#pragma once
#include "engine/meta/class_descriptor.hpp"

namespace codegen
{
    class SolGenerator
    {
      public:
        explicit SolGenerator(const std::string& output_filename);

        void generate(const std::vector<engine::meta::ClassDescriptor>& classes, const std::vector<engine::meta::FunctionDescriptor>& global_functions);

      private:
        std::string output_filename_;

        void generate_classes(const std::vector<engine::meta::ClassDescriptor>& classes, std::ofstream& out);
        void generate_global_functions(const std::vector<engine::meta::FunctionDescriptor>& global_functions, std::ofstream& out);
    };
} // namespace codegen