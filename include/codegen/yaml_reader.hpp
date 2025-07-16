#pragma once

#include <yaml-cpp/yaml.h>

namespace codegen
{
    class YamlReader final
    {
      public:
        explicit YamlReader(const std::string& filename);

        const std::unordered_map<std::string, std::vector<std::string>>& classes() const noexcept
        {
            return classes_;
        }

        const std::vector<std::string>& global_functions() const noexcept
        {
            return global_functions_;
        }

      private:
        std::unordered_map<std::string, std::vector<std::string>> classes_;
        std::vector<std::string> global_functions_;
    };
} // namespace codegen