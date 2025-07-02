#include "engine/serialization/json_serializer.hpp"

#include "pch.hpp"

#include <oatpp/core/parser/Caret.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

namespace engine::serialization
{
    json_serializer::json_serializer()
    {
        const auto serializer_config =
            oatpp::parser::json::mapping::Serializer::Config::createShared();

        const auto deserializer_config =
            oatpp::parser::json::mapping::Deserializer::Config::createShared();

        serializer_config->useBeautifier = true;

        object_mapper_ = oatpp::parser::json::mapping::ObjectMapper::createShared(
            serializer_config, deserializer_config);
    }

    oatpp::Object<dto::execution_request>
    json_serializer::from_json(const oatpp::String& json_str) const
    {
        if (json_str->empty())
        {
            return nullptr;
        }

        try
        {
            return object_mapper_->readFromString<oatpp::Object<dto::execution_request>>(json_str);
        }
        catch (const oatpp::parser::ParsingError& e)
        {
            return nullptr;
        }
    }

    oatpp::String
    json_serializer::to_json(const oatpp::Object<dto::execution_request>& request) const
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

    bool json_serializer::is_valid(const oatpp::String& json_str) const
    {
        if (json_str->empty())
        {
            return false;
        }

        try
        {
            oatpp::parser::Caret caret(json_str);
            object_mapper_->readFromCaret<oatpp::Object<dto::execution_request>>(caret);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
} // namespace engine::serialization