/**
 * Project: Engine
 * File: string_utils.hpp
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
 * @file string_utils.hpp
 * @brief Utility functions for string manipulation.
 */

#pragma once

#include <string>

namespace engine::utils::string
{
    /**
     * @brief Replaces all occurrences of a substring with another substring in the given string.
     * @param str The string to modify.
     * @param from The substring to be replaced.
     * @param to The substring to replace with.
     */
    inline void replace_all(std::string& str, const std::string& from, const std::string& to)
    {
        if (from.empty())
        {
            return;
        }

        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
} // engine::utils::string
