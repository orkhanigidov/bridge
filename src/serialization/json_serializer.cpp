#include "serialization/json_serializer.hpp"
#include <oatpp/core/parser/Caret.hpp>

namespace serialization {
    oatpp::Object<dto::ExecutionRequestDto>
    JsonSerializer::from_json(const oatpp::String& json_str) const
    {
        if (!is_valid_json(json_str)) {
            std::cerr << "Invalid JSON string provided" << std::endl;
            return nullptr;
        }

        try {
            return m_object_mapper
                    ->readFromString<oatpp::Object<dto::ExecutionRequestDto>>(
                            json_str);
        }
        catch (const oatpp::parser::ParsingError& e) {
            std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
            return nullptr;
        }
    }

    oatpp::String JsonSerializer::to_json(
            const oatpp::Object<dto::ExecutionRequestDto>& request) const
    {
        if (!request) { return nullptr; }

        try {
            return m_object_mapper->writeToString(request);
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to serialize request to JSON: " << e.what()
                      << std::endl;
            return nullptr;
        }
    }

    bool JsonSerializer::is_valid_json(const oatpp::String& json_str) const
    {
        if (!json_str || json_str->empty()) { return false; }

        try {
            oatpp::parser::Caret caret(json_str);
            m_object_mapper
                    ->readFromCaret<oatpp::Object<dto::ExecutionRequestDto>>(
                            caret);
            return true;
        }
        catch (...) {
            return false;
        }
    }
} // namespace serialization
