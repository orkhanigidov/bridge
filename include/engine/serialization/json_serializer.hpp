#pragma once

#include "engine/dto/execution_request.hpp"
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

namespace engine::serialization
{
    class JsonSerializer final
    {
    public:
        static JsonSerializer& instance()
        {
            static JsonSerializer instance;
            return instance;
        }

        oatpp::Object<dto::ExecutionRequest> from_json(const oatpp::String& json_str) const;
        oatpp::String to_json(const oatpp::Object<dto::ExecutionRequest>& request) const;

    private:
        JsonSerializer();

        std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> m_object_mapper;

        bool is_valid_json(const oatpp::String& json_str) const;
    };
} // namespace engine::serialization
