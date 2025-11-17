/**
 * Project: Engine
 * Tool: Codegen
 * File: header_collector.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file header_collector.hpp
 * @brief Declares the HeaderCollector for collecting header files from directories.
 */

#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

namespace codegen::io
{
    /**
     * @class HeaderCollectorException
     * @brief Exception thrown by HeaderCollector on errors.
     */
    class HeaderCollectorException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    /**
     * @class HeaderCollector
     * @brief Collects header files from specified directories and writes them to an output file.
     */
    class HeaderCollector final
    {
    public:
        /**
         * @brief Collects header files from the given include directories and writes their paths to an output file.
         * @param include_dirs The directories to search for header files.
         * @param output_file The file to write the collected header paths to.
         * @param extensions The file extensions to consider as headers (default: .h, .hpp).
         */
        static void collect_headers_to_file(const std::vector<std::filesystem::path>& include_dirs,
                                            const std::filesystem::path& output_file,
                                            const std::vector<std::string>& extensions = {".h", ".hpp"});

    private:
        HeaderCollector() = default;
    };
} // namespace codegen::io
