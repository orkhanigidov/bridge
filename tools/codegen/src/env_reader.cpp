#include "env_reader.hpp"

namespace codegen {

    EnvReader::EnvReader(const std::string& filename)
    {
        if (!filename.empty()) { load_file(filename); }
    }

    bool EnvReader::load_file(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open .env file: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            auto comment_pos = line.find('#');
            if (comment_pos != std::string::npos)
            {
                line.erase(comment_pos);
            }

            line = trim(line);
            if (line.empty())
            {
                continue;
            }

            auto equal_pos = line.find('=');
            if (equal_pos == std::string::npos)
            {
                continue;
            }

            std::string key = trim(line.substr(0, equal_pos));
            std::string value = trim(line.substr(equal_pos + 1));

            value = strip_quotes(value);
            m_env_vars.emplace(std::move(key), std::move(value));
        }
    }

    const std::string& EnvReader::get(const std::string& key) const
    {
        if (m_env_vars.contains(key))
        {
            return m_env_vars.at(key);
        }
    }

    std::string EnvReader::trim(const std::string& str)
    {
        auto start = str.find_first_not_of(" \t\r\n");
        auto end = str.find_last_not_of(" \t\r\n");
        return start == std::string::npos ? "" : str.substr(start, end - start + 1);
    }

    std::string EnvReader::strip_quotes(const std::string& str)
    {
        if (str.size() >= 2) {
            if ((str.front() == '"' && str.back() == '"') || (str.front() == '\'' && str.back() == '\'')) {
                return str.substr(1, str.size() - 2);
            }
        }
        return str;
    }

} // namespace codegen