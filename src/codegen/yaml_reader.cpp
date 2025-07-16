#include "codegen/yaml_reader.hpp"

namespace codegen
{
    YamlReader::YamlReader(const std::string& filename)
    {
        YAML::Node root = YAML::LoadFile(filename);

        if (const auto classes_node = root["classes"]; classes_node && classes_node.IsSequence())
        {
            for (const auto& class_ : classes_node)
            {
                const auto name_node = class_["name"];
                if (!name_node || !name_node.IsScalar())
                {
                    continue;
                }

                auto class_name = name_node.as<std::string>();
                std::vector<std::string> methods;

                if (const auto methods_node = class_["methods"]; methods_node && methods_node.IsSequence())
                {
                    methods.reserve(methods_node.size());
                    for (const auto& method : methods_node)
                    {
                        methods.emplace_back(method.as<std::string>());
                    }
                }
                classes_.emplace(std::move(class_name), std::move(methods));
            }
        }

        if (const auto global_functions_node = root["global_functions"]; global_functions_node && global_functions_node.IsSequence())
        {
            global_functions_.reserve(global_functions_node.size());
            for (const auto& global_function : global_functions_node)
            {
                global_functions_.emplace_back(global_function.as<std::string>());
            }
        }
    }
} // namespace codegen