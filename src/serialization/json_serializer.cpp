#include "serialization/json_serializer.hpp"

#include <oatpp/core/parser/Caret.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

namespace engine::serialization {
    
    oatpp::Object<network::dto::execution::RequestDto> JsonSerializer::from_json(const oatpp::String& json_data) const
    {
        if (!is_valid_json(json_data)) {
            std::cerr << "Deserialization failed: invalid JSON data" << std::endl;
            return nullptr;
        }

        try {
            return m_object_mapper->readFromString<oatpp::Object<network::dto::execution::RequestDto>>(json_data);
        } catch (const oatpp::parser::ParsingError& e) {
            std::cerr << "JSON deserialization error: " << e.what() << std::endl;
            return nullptr;
        }
    }

    oatpp::String JsonSerializer::to_json(const oatpp::Object<network::dto::execution::RequestDto>& request_dto) const
    {
        if (!request_dto) {
            std::cerr << "Serialization failed: invalid request object" << std::endl;
            return nullptr;
        }

        try {
            return m_object_mapper->writeToString(request_dto);
        } catch (const std::exception& e) {
            std::cerr << "JSON serialization error: " << e.what() << std::endl;
            return nullptr;
        }
    }

    bool JsonSerializer::is_valid_json(const oatpp::String& json_data) const
    {
        if (!json_data || json_data->empty()) {
            return false;
        }

        try {
            oatpp::parser::Caret caret(json_data);
            m_object_mapper->getDeserializer()->deserialize(caret, nullptr);
            return !caret.hasError();
        } catch (...) {
            return false;
        }
    }

} // namespace engine::serialization
