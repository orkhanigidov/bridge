#pragma once

namespace codegen::io
{
    class EnvReaderException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class EnvReader final
    {
    public:
        explicit EnvReader(const fs::path& env_path);

        std::optional<std::string> get(std::string_view key) const;

    private:
        std::unordered_map<std::string, std::string> env_vars_;

        void from_file(const fs::path& env_path);
    };
} // namespace codegen::io
