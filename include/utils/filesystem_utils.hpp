/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
