#include "io/yaml_reader.hpp"

namespace codegen::io
{
    bool YamlReader::load_file(const std::string& filename)
    {
        classes_.clear();
        free_functions_.clear();

        YAML::Node root;
        try
        {
            root = YAML::LoadFile(filename);
        }
        catch (const YAML::BadFile&)
        {
            std::cerr << "Failed to open YAML file: " << filename << std::endl;
            return false;
        }
        catch (const YAML::ParserException&)
        {
            std::cerr << "Failed to parse YAML file: " << filename << std::endl;
            return false;
        }

        if (!extract_classes(root) || !extract_free_functions(root))
        {
            std::cerr << "Invalid YAML structure in file: " << filename << std::endl;
            return false;
        }
        return true;
    }

    bool YamlReader::extract_classes(const YAML::Node& root)
    {
        const auto& classes_node = root[k_classes];
        if (!classes_node || !classes_node.IsSequence())
        {
            return false;
        }

        for (const auto& class_node : classes_node)
        {
            const auto& name_node = class_node[k_name];
            if (!name_node || !name_node.IsScalar())
            {
                continue;
            }
            auto method_names = extract_methods(class_node);
            classes_.emplace(name_node.as<std::string>(), std::move(method_names));
        }
        return true;
    }

    std::vector<std::string> YamlReader::extract_methods(const YAML::Node& node)
    {
        std::vector<std::string> method_names;
        if (const auto& methods_node = node[k_methods]; methods_node && methods_node.IsSequence())
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

    bool YamlReader::extract_free_functions(const YAML::Node& root)
    {
        const auto& globals_node = root[k_free_functions];
        if (!globals_node || !globals_node.IsSequence())
        {
            return false;
        }

        free_functions_.reserve(globals_node.size());
        for (const auto& func_node : globals_node)
        {
            if (func_node.IsScalar())
            {
                free_functions_.emplace_back(func_node.as<std::string>());
            }
        }
        return true;
    }
} // namespace codegen::io
