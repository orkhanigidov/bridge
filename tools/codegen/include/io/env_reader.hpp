/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
