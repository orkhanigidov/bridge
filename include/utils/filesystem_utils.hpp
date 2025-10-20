#pragma once

namespace engine::utils::filesystem
{
    inline std::string to_forward_slashes(const fs::path& path)
    {
        return path.generic_string();
    }
} // engine::utils::filesystem
