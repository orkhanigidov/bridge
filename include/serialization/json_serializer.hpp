#pragma once

#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

#include "network/dto/execution/request_dto.hpp"

namespace engine::serialization {

    class JsonSerializer final {
    public:
        explicit JsonSerializer(OATPP_COMPONENT(std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper>, object_mapper))
            : object_mapper_(object_mapper) {}

        oatpp::Object<network::dto::execution::RequestDto> from_json(const oatpp::String& json_data) const;
        oatpp::String to_json(const oatpp::Object<network::dto::execution::RequestDto>& request_dto) const;

    private:
        std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> object_mapper_;

        bool is_valid_json(const oatpp::String& json_data) const;
    };

} // namespace engine::serialization
