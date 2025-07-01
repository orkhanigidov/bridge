#pragma once

#include "dto/ExecutionRequest.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

namespace engine::serialization
{
    class Serializer
    {
      public:
        static Serializer& instance()
        {
            static Serializer instance;
            return instance;
        }
        ~Serializer() = default;

        oatpp::Object<dto::ExecutionRequest> from_json(const oatpp::String& json_str) const;
        oatpp::String to_json(const oatpp::Object<dto::ExecutionRequest>& request) const;

        bool is_valid(const oatpp::String& json_str) const;

      private:
        std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> object_mapper_;

        Serializer();
    };
} // namespace engine::serialization