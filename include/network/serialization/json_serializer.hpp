#pragma once

#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

namespace engine::network::serialization
{
    class JsonSerializerException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    template <typename DtoType>
    class JsonSerializer final
    {
    public:
        explicit JsonSerializer(std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> object_mapper): object_mapper_(std::move(object_mapper))
        {
            if (!object_mapper_)
            {
                throw JsonSerializerException("Failed to create JSON serializer: object mapper is null.");
            }
        }

        DtoType from_json(const oatpp::String& json_data) const
        {
            if (!json_data || json_data->empty())
            {
                throw JsonSerializerException("Deserialization failed: input JSON data is empty or null.");
            }

            try
            {
                return object_mapper_->readFromString<DtoType>(json_data);
            } catch (const oatpp::parser::ParsingError& e)
            {
                throw JsonSerializerException(std::format("JSON deserialization error: {}", e.what()));
            }
        }

        oatpp::String to_json(const DtoType& dto) const
        {
            if (!dto)
            {
                throw JsonSerializerException("Serialization failed: input DTO object is null.");
            }

            try
            {
                return object_mapper_->writeToString(dto);
            } catch (const std::exception& e)
            {
                throw JsonSerializerException(std::format("JSON serialization error: {}", e.what()));
            }
        }

    private:
        std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> object_mapper_;
    };
} // namespace engine::network::serialization
