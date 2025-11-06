/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
