#pragma once

#include "engine/meta/class_descriptor.hpp"
#include "pch.hpp"

#include <clang-c/Index.h>

namespace codegen
{
    class ClangAnalyzer final
    {
      public:
        explicit ClangAnalyzer(const std::string& filename, const std::unordered_map<std::string, std::vector<std::string>>& target_classes, const std::vector<std::string>& target_global_functions);

        const std::vector<engine::meta::ClassDescriptor>& foundClasses()
        {
            return found_classes_;
        }

        const std::vector<engine::meta::FunctionDescriptor>& foundGlobalFunctions()
        {
            return found_global_functions_;
        }

      private:
        std::unordered_map<std::string, std::vector<std::string>> target_classes_;
        std::vector<std::string> target_global_functions_;
        std::vector<engine::meta::ClassDescriptor> found_classes_;
        std::vector<engine::meta::FunctionDescriptor> found_global_functions_;

        static std::string getSpelling(const CXCursor& cursor);
        static std::string getResultType(const CXCursor& cursor);
        static std::string getType(const CXCursor& cursor);
        static std::vector<engine::meta::ParameterDescriptor> getArguments(const CXCursor& cursor);
        static CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data);
    };
} // namespace codegen