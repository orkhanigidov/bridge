/**
 * Project: Engine
 * File: temp_file_guard.hpp
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
 * @file temp_file_guard.hpp
 * @brief Declares the TempFileGuard utility for managing temporary file lifetimes.
 */

#pragma once

#include <filesystem>
#include <system_error>
#include <utility>

namespace engine::utils
{
    /**
     * @class TempFileGuard
     * @brief RAII guard that deletes a temporary file when it goes out of scope.
     *
     * Ensures the specified file is removed in the destructor if it still exists.
     * Errors during removal are ignored.
     */
    class TempFileGuard final
    {
    public:
        /**
         * @brief Constructs the guard for the given file path.
         * @param path The path to the temporary file.
         */
        explicit TempFileGuard(std::filesystem::path path) : path_(std::move(path))
        {
        }

        /**
         * @brief Destructor. Removes the file if it exists. Errors are ignored.
         */
        ~TempFileGuard()
        {
            std::error_code ec;
            if (std::filesystem::exists(path_, ec))
            {
                std::filesystem::remove(path_, ec);
            }
        }

    private:
        /**
         * @brief Path to the temporary file.
         */
        std::filesystem::path path_;
    };
} // namespace engine::utils
