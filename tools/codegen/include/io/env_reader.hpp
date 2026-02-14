/**
 * Project: Engine
 * Tool: Codegen
 * File: env_reader.hpp
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
 * @file env_reader.hpp
 * @brief Declares the EnvReader for reading environment variables from a file.
 */

#pragma once

#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

namespace codegen::io
{
    /**
     * @class EnvReaderException
     * @brief Exception thrown by EnvReader on errors.
     */
    class EnvReaderException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    /**
     * @class EnvReader
     * @brief Reads environment variables from a file into a map.
     *
     * Provides access to environment variables loaded from a specified file.
     */
    class EnvReader final
    {
    public:
        /**
         * @brief Constructs an EnvReader and loads environment variables from a file.
         * @param env_path Path to the environment file.
         */
        explicit EnvReader(const std::filesystem::path& env_path);

        /**
         * @brief Gets the value of an environment variable by key.
         * @param key The environment variable name.
         * @return The value if found, std::nullopt otherwise.
         */
        std::optional<std::string> get(std::string_view key) const;

    private:
        /**
         * @brief Map of environment variables.
         */
        std::unordered_map<std::string, std::string> env_vars_;

        /**
         * @brief Loads environment variables from a file.
         * @param env_path Path to the environment file.
         */
        void from_file(const std::filesystem::path& env_path);
    };
} // namespace codegen::io
