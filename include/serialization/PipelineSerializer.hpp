#pragma once

#include "dto/RequestDTO.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "pch.hpp"

namespace engine::serialization
{
    class PipelineSerializer final
    {
      public:
        static oatpp::Object<dto::RequestDTO> from_json(const oatpp::String& json_str);
        static oatpp::String to_json(const oatpp::Object<dto::RequestDTO>& request);

        static bool is_valid(const oatpp::String& json_str);

      private:
        static std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> get_object_mapper();
    };
} // namespace engine::serialization