#include "io/env_reader.hpp"

namespace
{
    std::string trim(const std::string& str)
    {
        const auto start = str.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
        {
            return "";
        }
        const auto end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }

    std::string strip_quotes(const std::string& str)
    {
        if (str.length() >= 2)
        {
            if ((str.front() == '"' && str.back() == '"') || (str.front() == '\'' && str.back() == '\''))
            {
                return str.substr(1, str.length() - 2);
            }
        }
        return str;
    }
}

namespace codegen::io
{
    EnvReader::EnvReader(const std::string& filename)
    {
#ifdef PROJECT_ROOT
        const fs::path& env_path = fs::path(PROJECT_ROOT) / filename;
#else
        throw std::runtime_error("PROJECT_ROOT environment variable is not defined.");
#endif

        std::cout << "Loading .env file: " << env_path.string() << std::endl;
        load_file(env_path.string());
    }

    void EnvReader::load_file(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open .env file: " + std::string(filename));
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (const auto comment_pos = line.find('#'); comment_pos != std::string::npos)
            {
                line.erase(comment_pos);
            }

            line = trim(line);
            if (line.empty())
            {
                continue;
            }

            const auto equal_pos = line.find('=');
            if (equal_pos == std::string::npos)
            {
                continue;
            }

            auto key = trim(line.substr(0, equal_pos));
            auto value = trim(line.substr(equal_pos + 1));
            value = strip_quotes(value);

            if (!key.empty())
            {
                env_vars_.emplace(key, value);
            }
        }
    }

    std::optional<std::string> EnvReader::get(const std::string& key) const
    {
        if (const auto it = env_vars_.find(key); it != env_vars_.end())
        {
            return it->second;
        }
        return std::nullopt;
    }
} // namespace codegen::io
