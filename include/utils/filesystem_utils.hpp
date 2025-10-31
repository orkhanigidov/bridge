/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <filesystem>
#include <string>

namespace engine::utils::filesystem
{
    inline std::string to_forward_slashes(const std::filesystem::path& path)
    {
        return path.generic_string();
    }
} // engine::utils::filesystem
