namespace engine::utils::filesystem
{
    std::string
    to_forward_slashes(const fs::path& path)
    {
        std::string path_str = path.string();
        std::ranges::replace(path_str, '\\', '/');
        return path_str;
    }
}
