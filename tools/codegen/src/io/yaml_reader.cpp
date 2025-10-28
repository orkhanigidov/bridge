#include "io/yaml_reader.hpp"

#include <string>
#include <utility>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "analysis/analysis_data.hpp"

namespace
{
    std::vector<std::string> extract_string_sequence(const YAML::Node& root, const char* key)
    {
        std::vector<std::string> names;
        const auto& node = root[key];

        if (node && node.IsSequence())
        {
            names.reserve(node.size());
            for (const auto& item_node : node)
            {
                if (item_node.IsScalar())
                {
                    names.emplace_back(item_node.as<std::string>());
                }
            }
        }
        return names;
    }
}

namespace codegen::io
{
    YamlReader YamlReader::from_file(const std::filesystem::path& file_path)
    {
        YAML::Node root;
        try
        {
            root = YAML::LoadFile(file_path.string());
        } catch (const YAML::BadFile&)
        {
            throw YamlReaderException(std::format("Could not open YAML file: {}", file_path.string()));
        }
        catch (const YAML::ParserException& e)
        {
            throw YamlReaderException(std::format("Failed to parse YAML file: {}", file_path.string(), e.what()));
        }

        return YamlReader(root);
    }

    YamlReader::YamlReader(const YAML::Node& root)
    {
        extract_all_classes(root);
        free_functions_ = extract_string_sequence(root, FREE_FUNCTIONS);
        enums_ = extract_string_sequence(root, ENUMS);
    }

    void YamlReader::extract_all_classes(const YAML::Node& root)
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
            auto class_name = name_node.as<std::string>();
            analysis::ClassConfig config;

            config.methods = extract_string_sequence(class_node, METHODS);
            config.types = extract_string_sequence(class_node, TYPES);

            classes_.emplace(std::move(class_name), std::move(config));
        }
    }
} // namespace codegen::io
