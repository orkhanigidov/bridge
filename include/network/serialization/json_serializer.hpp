/**
 * Project: Engine
 * File: json_serializer.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file json_serializer.hpp
 * @brief Defines the JsonSerializer for serializing and deserializing DTOs using Oat++ JSON mapping.
 */

#pragma once

#include <exception>
#include <memory>
#include <stdexcept>
#include <utility>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/parser/ParsingError.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

namespace engine::network::serialization
{
    /**
     * @class JsonSerializerException
     * @brief Exception type for JSON serialization and deserialization errors.
     */
    class JsonSerializerException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    /**
     * @class JsonSerializer
     * @brief Generic JSON serializer/deserializer for Oat++ DTOs.
     * @tparam DtoType The DTO type to serialize/deserialize.
     *
     * Provides methods to serialize DTOs to JSON strings and deserialize JSON strings to DTOs.
     * Throws JsonSerializerException on error.
     */
    template <typename DtoType>
    class JsonSerializer final
    {
    public:
        /**
         * @brief Constructs a JsonSerializer with the given object mapper.
         * @param object_mapper Shared pointer to an Oat++ JSON object mapper.
         * @throws JsonSerializerException if the object mapper is null.
         */
        explicit JsonSerializer(std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> object_mapper) : object_mapper_(std::move(object_mapper))
        {
            if (!object_mapper_)
            {
                throw JsonSerializerException("Failed to create JSON serializer: object mapper is null.");
            }
        }

        /**
         * @brief Deserializes a JSON string to a DTO object.
         * @param json_data The JSON string to deserialize.
         * @return The deserialized DTO object.
         * @throws JsonSerializerException if input is empty/null, or parsing fails.
         */
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
                throw JsonSerializerException("JSON deserialization error: " + std::string(e.what()));
            }
        }

        /**
         * @brief Serializes a DTO object to a JSON string.
         * @param dto The DTO object to serialize.
         * @return The serialized JSON string.
         * @throws JsonSerializerException if input is null or serialization fails.
         */
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
                throw JsonSerializerException("JSON serialization error: " + std::string(e.what()));
            }
        }

    private:
        /**
         * @brief The Oat++ JSON object mapper.
         */
        std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> object_mapper_;
    };
} // namespace engine::network::serialization
