#include "serialization/PipelineSerializer.hpp"

#include "oatpp/core/parser/Caret.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "pch.hpp"

namespace engine::serialization
{
    oatpp::Object<dto::RequestDTO> PipelineSerializer::from_json(const oatpp::String& json_str)
    {
        if (!json_str || json_str->empty())
        {
            return nullptr;
        }

        try
        {
            const auto object_mapper = get_object_mapper();
            return object_mapper->readFromString<oatpp::Object<dto::RequestDTO>>(json_str);
        }
        catch (const oatpp::parser::ParsingError& e)
        {
            return nullptr;
        }
    }

    oatpp::String PipelineSerializer::to_json(const oatpp::Object<dto::RequestDTO>& request)
    {
        if (!request)
        {
            return nullptr;
        }

        try
        {
            const auto object_mapper = get_object_mapper();
            return object_mapper->writeToString(request);
        }
        catch (const std::exception& e)
        {
            return nullptr;
        }
    }

    bool PipelineSerializer::is_valid(const oatpp::String& json_str)
    {
        if (!json_str || json_str->empty())
        {
            return false;
        }

        try
        {
            oatpp::parser::Caret caret(json_str);
            const auto object_mapper = get_object_mapper();
            object_mapper->readFromCaret<oatpp::Object<dto::RequestDTO>>(caret);
            return true;
        }
        catch (const oatpp::parser::ParsingError&)
        {
            return false;
        }
    }

    std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper>
    PipelineSerializer::get_object_mapper()
    {
        static auto object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        return object_mapper;
    }
} // namespace engine::serialization