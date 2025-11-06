/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file options_dto.hpp
 * @brief Defines the OptionsDto DTO for execution options in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class OptionsDto
     * @brief Data transfer object for execution options.
     *
     * Contains options such as timeout and output data format for execution requests.
     */
    class OptionsDto final : public oatpp::DTO
    {
        DTO_INIT(OptionsDto, DTO)

        /**
         * @brief Timeout for execution in milliseconds.
         */
        DTO_FIELD(oatpp::UInt32, timeout_milliseconds);

        /**
         * @brief Desired output data format (e.g., "graphml").
         */
        DTO_FIELD(oatpp::String, output_data_format);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
