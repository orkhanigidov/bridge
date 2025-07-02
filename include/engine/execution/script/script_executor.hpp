#pragma once

#include "pch.hpp"

namespace engine::execution
{
    class script_executor final
    {
      public:
        static void execute_from_file(const std::filesystem::path& script_path);
        static void execute_from_string(std::string_view script_content);

      private:
        static std::filesystem::path normalize_path(const std::filesystem::path& path);
        static void validate_path(const std::filesystem::path& path);
    };
} // namespace engine::execution