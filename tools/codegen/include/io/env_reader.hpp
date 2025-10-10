#pragma once

namespace codegen::io
{
    class EnvReader final
    {
    public:
        explicit EnvReader(const std::string& filename = ".env");

        const std::string& get(const std::string& key) const;

    private:
        std::unordered_map<std::string, std::string> env_vars_;

        bool load_file(const std::string& filename);
        static std::string trim(const std::string& str);
        static std::string strip_quotes(const std::string& str);
    };
} // namespace codegen::io
