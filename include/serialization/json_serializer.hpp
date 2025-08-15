#pragma once

#include "network/dto/execution_request_dto.hpp"
#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/component.hpp>

namespace serialization {
    class JsonSerializer final {
    public:
        explicit JsonSerializer(OATPP_COMPONENT(
                std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                object_mapper))
            : m_object_mapper(std::move(object_mapper))
        {}

        oatpp::Object<dto::ExecutionRequestDto>
        from_json(const oatpp::String& json_str) const;
        oatpp::String
        to_json(const oatpp::Object<dto::ExecutionRequestDto>& request) const;

    private:
        std::shared_ptr<oatpp::data::mapping::ObjectMapper> m_object_mapper;

        bool is_valid_json(const oatpp::String& json_str) const;
    };
} // namespace serialization
