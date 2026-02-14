/**
 * Project: Engine
 * File: filesystem_utils.hpp
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
 * @file filesystem_utils.hpp
 * @brief Utility functions for filesystem path manipulation.
 */

#pragma once

#include <filesystem>
#include <string>

namespace engine::utils::filesystem
{
    /**
     * @brief Converts a filesystem path to a string with forward slashes.
     * @param path The filesystem path.
     * @return The path as a string using forward slashes as separators.
     */
    inline std::string to_forward_slashes(const std::filesystem::path& path)
    {
        return path.generic_string();
    }
} // engine::utils::filesystem
