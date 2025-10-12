#include "io/env_reader.hpp"

namespace
{
    const std::string& trim(const std::string& sv)
    {
        const auto start = sv.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
        {
            return {};
        }
        const auto end = sv.find_last_not_of(" \t\r\n");
        return sv.substr(start, end - start + 1);
    }

    const std::string& strip_quotes(const std::string& sv)
    {
        if (sv.length() >= 2)
        {
            if ((sv.front() == '"' && sv.back() == '"') || (sv.front() == '\'' && sv.back() == '\''))
            {
                return sv.substr(1, sv.length() - 2);
            }
        }
        return sv;
    }
}

namespace codegen::io
{
    EnvReader::EnvReader(const std::string& filename)
    {
        load_file(filename);
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

    std::optional<const std::string&> EnvReader::get(const std::string& key) const
    {
        if (const auto it = env_vars_.find(key); it != env_vars_.end())
        {
            return it->second;
        }
        return std::nullopt;
    }
} // namespace codegen::io
