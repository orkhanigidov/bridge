/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file metadata_dto.hpp
 * @brief Defines the MetadataDto DTO for execution metadata in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class MetadataDto
     * @brief Data transfer object for execution metadata.
     *
     * Contains metadata such as the duration of execution in milliseconds.
     */
    class MetadataDto final : public oatpp::DTO
    {
        DTO_INIT(MetadataDto, DTO)

        /**
         * @brief Duration of execution in milliseconds.
         */
        DTO_FIELD(oatpp::UInt64, duration_milliseconds);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
