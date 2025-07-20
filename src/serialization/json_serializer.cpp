#include "engine/serialization/json_serializer.hpp"

namespace engine::serialization
{
    JsonSerializer::JsonSerializer()
    {
        const auto serializer_config = oatpp::parser::json::mapping::Serializer::Config::createShared();
        const auto deserializer_config = oatpp::parser::json::mapping::Deserializer::Config::createShared();

        serializer_config->useBeautifier = true;

        m_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared(
            serializer_config, deserializer_config);
    }

    oatpp::Object<dto::ExecutionRequest> JsonSerializer::from_json(const oatpp::String& json_str) const
    {
        if (!is_valid_json(json_str))
        {
            std::cerr << "Invalid JSON string provided" << std::endl;
            return nullptr;
        }

        try
        {
            return m_object_mapper->readFromString<oatpp::Object<dto::ExecutionRequest>>(json_str);
        }
        catch (const oatpp::parser::ParsingError& e)
        {
            std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
            return nullptr;
        }
    }

    oatpp::String JsonSerializer::to_json(const oatpp::Object<dto::ExecutionRequest>& request) const
    {
        if (!request)
        {
            return nullptr;
        }

        try
        {
            return m_object_mapper->writeToString(request);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to serialize request to JSON: " << e.what() << std::endl;
            return nullptr;
        }
    }

    bool JsonSerializer::is_valid_json(const oatpp::String& json_str) const
    {
        if (!json_str || json_str->empty())
        {
            return false;
        }

        try
        {
            oatpp::parser::Caret caret(json_str);
            m_object_mapper->readFromCaret<oatpp::Object<dto::ExecutionRequest>>(caret);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
} // namespace engine::serialization
