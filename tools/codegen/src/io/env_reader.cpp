/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#include "io/env_reader.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>

namespace
{
    constexpr std::string_view trim(std::string_view str) noexcept
    {
        constexpr std::string_view whitespace = " \t\r\n";
        const auto start = str.find_first_not_of(whitespace);
        if (start == std::string_view::npos)
        {
            return {};
        }
        const auto end = str.find_last_not_of(whitespace);
        return str.substr(start, end - start + 1);
    }

    constexpr std::string_view strip_quotes(std::string_view str) noexcept
    {
        if (str.length() >= 2)
        {
            if ((str.starts_with('"') && str.ends_with('"')) ||
                (str.starts_with('\'') && str.ends_with('\'')))
            {
                str.remove_prefix(1);
                str.remove_suffix(1);
            }
        }
        return str;
    }
}

namespace codegen::io
{
    EnvReader::EnvReader(const std::filesystem::path& env_path)
    {
        from_file(env_path);
    }

    void EnvReader::from_file(const std::filesystem::path& env_path)
    {
        std::ifstream file(env_path);
        if (!file.is_open())
        {
            throw EnvReaderException(std::format("Could not open .env file: {}", env_path.string()));
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::string_view line_view = line;

            if (const auto comment_pos = line.find('#'); comment_pos != std::string_view::npos)
            {
                line_view.remove_suffix(line_view.length() - comment_pos);
            }

            line_view = trim(line_view);
            if (line_view.empty())
            {
                continue;
            }

            const auto equal_pos = line_view.find('=');
            if (equal_pos == std::string_view::npos)
            {
                continue;
            }

            auto key_view = trim(line_view.substr(0, equal_pos));
            auto value_view = strip_quotes(trim(line_view.substr(equal_pos + 1)));

            if (!key_view.empty())
            {
                env_vars_.emplace(std::string(key_view), std::string(value_view));
            }
        }
    }

    std::optional<std::string> EnvReader::get(std::string_view key) const
    {
        if (const auto it = env_vars_.find(std::string(key)); it != env_vars_.end())
        {
            return it->second;
        }
        return std::nullopt;
    }
} // namespace codegen::io
