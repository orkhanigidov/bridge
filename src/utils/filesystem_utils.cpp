namespace engine::utils::filesystem
{
    std::string to_forward_slashes(const fs::path& path)
    {
        return path.generic_string();
    }
} // engine::utils::filesystem
