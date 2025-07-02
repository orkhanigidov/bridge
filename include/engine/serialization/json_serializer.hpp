#pragma once

#include "engine/dto/execution_request.hpp"

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

namespace engine::serialization
{
    class json_serializer final
    {
      public:
        static json_serializer& instance()
        {
            static json_serializer instance;
            return instance;
        }

        oatpp::Object<dto::execution_request> from_json(const oatpp::String& json_str) const;
        oatpp::String to_json(const oatpp::Object<dto::execution_request>& request_dto) const;

        bool is_valid(const oatpp::String& json_str) const;

      private:
        std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> object_mapper_;

        json_serializer();
    };
} // namespace engine::serialization