#include "codegen/yaml_reader.hpp"

namespace codegen {
    bool YamlReader::load_file(const std::string& filename)
    {
        m_classes.clear();
        m_free_functions.clear();

        YAML::Node root;
        try {
            root = YAML::LoadFile(filename);
        }
        catch (const YAML::BadFile&) {
            std::cerr << "Failed to open YAML file: " << filename << std::endl;
            return false;
        }
        catch (const YAML::ParserException&) {
            std::cerr << "Failed to parse YAML file: " << filename << std::endl;
            return false;
        }

        if (!extract_classes(root) || !extract_free_functions(root)) {
            std::cerr << "Invalid YAML structure in file: " << filename << std::endl;
            return false;
        }
        return true;
    }

    bool YamlReader::extract_classes(const YAML::Node& root)
    {
        const auto& classes_node = root[kClasses];
        if (!classes_node || !classes_node.IsSequence()) { return false; }

        for (const auto& class_node: classes_node) {
            const auto& name_node = class_node[kName];
            if (!name_node || !name_node.IsScalar()) { continue; }
            auto method_names = extract_methods(class_node);
            m_classes.emplace(name_node.as<std::string>(), std::move(method_names));
        }
        return true;
    }

    std::vector<std::string> YamlReader::extract_methods(const YAML::Node& node)
    {
        std::vector<std::string> method_names;
        if (const auto& methods_node = node[kMethods]; methods_node && methods_node.IsSequence()) {
            method_names.reserve(methods_node.size());
            for (const auto& method_node: methods_node) {
                if (method_node.IsScalar()) { method_names.emplace_back(method_node.as<std::string>()); }
            }
        }
        return method_names;
    }

    bool YamlReader::extract_free_functions(const YAML::Node& root)
    {
        const auto& globals_node = root[kFreeFunctions];
        if (!globals_node || !globals_node.IsSequence()) { return false; }

        m_free_functions.reserve(globals_node.size());
        for (const auto& func_node: globals_node) {
            if (func_node.IsScalar()) { m_free_functions.emplace_back(func_node.as<std::string>()); }
        }
        return true;
    }
} // namespace codegen
