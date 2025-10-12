#pragma once

namespace codegen::io
{
    class EnvReader final
    {
    public:
        explicit EnvReader(const std::string& filename = ".env");

        std::optional<std::string> get(const std::string& key) const;

    private:
        std::unordered_map<std::string, std::string> env_vars_;

        void load_file(const std::string& filename);
    };
} // namespace codegen::io
