#include "io/yaml_reader.hpp"

namespace codegen::io
{
    YamlReader YamlReader::from_file(const std::string& filename)
    {
        YAML::Node root;
        try
        {
            root = YAML::LoadFile(filename);
        }
        catch (const YAML::BadFile&)
        {
            throw YamlReaderException("Failed to open YAML file: " + filename);
        }
        catch (const YAML::ParserException&)
        {
            throw YamlReaderException("Failed to parse YAML file: " + filename);
        }

        return YamlReader(root);
    }

    YamlReader::YamlReader(const YAML::Node& root)
    {
        extract_classes(root);
        extract_free_functions(root);
    }

    void YamlReader::extract_classes(const YAML::Node& root)
    {
        const auto& classes_node = root[CLASSES];
        if (!classes_node || !classes_node.IsSequence())
        {
            return;
        }

        for (const auto& class_node : classes_node)
        {
            const auto& name_node = class_node[NAME];
            if (!name_node || !name_node.IsScalar())
            {
                continue;
            }
            auto method_names = extract_methods(class_node);
            classes_.emplace(name_node.as<std::string>(), std::move(method_names));
        }
    }

    std::vector<std::string> YamlReader::extract_methods(const YAML::Node& node)
    {
        std::vector<std::string> method_names;
        const auto& methods_node = node[METHODS];
        if (methods_node && methods_node.IsSequence())
        {
            method_names.reserve(methods_node.size());
            for (const auto& method_node : methods_node)
            {
                if (method_node.IsScalar())
                {
                    method_names.emplace_back(method_node.as<std::string>());
                }
            }
        }
        return method_names;
    }

    void YamlReader::extract_free_functions(const YAML::Node& root)
    {
        const auto& globals_node = root[FREE_FUNCTIONS];
        if (!globals_node || !globals_node.IsSequence())
        {
            return;
        }

        free_functions_.reserve(globals_node.size());
        for (const auto& func_node : globals_node)
        {
            if (func_node.IsScalar())
            {
                free_functions_.emplace_back(func_node.as<std::string>());
            }
        }
    }
} // namespace codegen::io
