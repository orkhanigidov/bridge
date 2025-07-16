#pragma once

#include "pch.hpp"

namespace codegen
{
    class HeaderIncluder
    {
      public:
        explicit HeaderIncluder(const std::string& include_dir, const std::string& output_filename);

      private:
        std::string include_dir_;
        std::string output_filename_;

        static bool isHeaderFile(const std::filesystem::path& path);
    };
} // namespace codegen