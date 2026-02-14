/**
 * Project: Engine
 * Tool: Codegen
 * File: env_reader.cpp
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
 * @file env_reader.cpp
 * @brief Implements the EnvReader for reading environment variables from a file.
 */

#include "io/env_reader.hpp"

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>

namespace
{
    /**
     * @brief Trims whitespace from both ends of a string view.
     * @param str The string view to trim.
     * @return Trimmed string view.
     */
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

    /**
     * @brief Removes surrounding single or double quotes from a string view.
     * @param str The string view to process.
     * @return String view without surrounding quotes.
     */
    constexpr std::string_view strip_quotes(std::string_view str) noexcept
    {
        if (str.length() >= 2)
        {
            if ((str.front() == '"' && str.back() == '"') || (str.front() == '\'' && str.back() == '\''))
            {
                return str.substr(1, str.length() - 2);
            }
        }
        return str;
    }
}

namespace codegen::io
{
    /**
     * @brief Constructs an EnvReader and loads environment variables from a file.
     * @param env_path Path to the environment file.
     */
    EnvReader::EnvReader(const std::filesystem::path& env_path)
    {
        from_file(env_path);
    }

    /**
     * @brief Loads environment variables from a file into the internal map.
     * @param env_path Path to the environment file.
     * @throws EnvReaderException if the file cannot be opened.
     */
    void EnvReader::from_file(const std::filesystem::path& env_path)
    {
        std::ifstream file(env_path);
        if (!file.is_open())
        {
            throw EnvReaderException("Could not open .env file: " + env_path.string());
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
                env_vars_.try_emplace(std::string(key_view), value_view);
            }
        }
    }

    /**
     * @brief Gets the value of an environment variable by key.
     * @param key The environment variable name.
     * @return The value if found, std::nullopt otherwise.
     */
    std::optional<std::string> EnvReader::get(std::string_view key) const
    {
        if (const auto it = env_vars_.find(std::string(key)); it != env_vars_.end())
        {
            return it->second;
        }
        return std::nullopt;
    }
} // namespace codegen::io
