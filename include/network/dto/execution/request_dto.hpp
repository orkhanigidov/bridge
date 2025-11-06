/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file request_dto.hpp
 * @brief Defines the RequestDto DTO for execution requests in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "file_dto.hpp"
#include "options_dto.hpp"
#include "type_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @class RequestDto
     * @brief Data transfer object for execution requests.
     *
     * Contains the execution type, script, input data files, and execution options.
     */
    class RequestDto final : public oatpp::DTO
    {
        DTO_INIT(RequestDto, DTO)

        /**
         * @brief The type of execution to perform.
         */
        DTO_FIELD(oatpp::Enum<ExecutionTypeDto>::AsString, type);

        /**
         * @brief The script to the executed.
         */
        DTO_FIELD(oatpp::String, script);

        /**
         * @brief List of input data files as file chunks.
         */
        DTO_FIELD(oatpp::List<oatpp::Object<FileDto>>, input_data);

        /**
         * @brief Execution options such as timeout and output format.
         */
        DTO_FIELD(oatpp::Object<OptionsDto>, options);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
