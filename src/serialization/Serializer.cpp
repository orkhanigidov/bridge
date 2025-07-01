#include "serialization/Serializer.hpp"

#include "oatpp/core/parser/Caret.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "pch.hpp"

namespace engine::serialization
{
    Serializer::Serializer()
    {
        const auto serializer_config =
            oatpp::parser::json::mapping::Serializer::Config::createShared();

        const auto deserializer_config =
            oatpp::parser::json::mapping::Deserializer::Config::createShared();

        serializer_config->useBeautifier = true;

        object_mapper_ = oatpp::parser::json::mapping::ObjectMapper::createShared(
            serializer_config, deserializer_config);
    }

    oatpp::Object<dto::ExecutionRequest> Serializer::from_json(const oatpp::String& json_str) const
    {
        if (!json_str || json_str->empty())
        {
            return nullptr;
        }

        try
        {
            return object_mapper_->readFromString<oatpp::Object<dto::ExecutionRequest>>(json_str);
        }
        catch (const oatpp::parser::ParsingError& e)
        {
            return nullptr;
        }
    }

    oatpp::String Serializer::to_json(const oatpp::Object<dto::ExecutionRequest>& request) const
    {
        if (!request)
        {
            return nullptr;
        }

        try
        {
            return object_mapper_->writeToString(request);
        }
        catch (const std::exception& e)
        {
            return nullptr;
        }
    }

    bool Serializer::is_valid(const oatpp::String& json_str) const
    {
        if (!json_str || json_str->empty())
        {
            return false;
        }

        try
        {
            oatpp::parser::Caret caret(json_str);
            object_mapper_->readFromCaret<oatpp::Object<dto::ExecutionRequest>>(caret);
            return true;
        }
        catch (const oatpp::parser::ParsingError&)
        {
            return false;
        }
    }
} // namespace engine::serialization