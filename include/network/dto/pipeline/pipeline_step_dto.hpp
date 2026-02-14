/**
 * Project: Engine
 * File: pipeline_step_dto.hpp
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
 * @file pipeline_step_dto.hpp
 * @brief Defines the PipelineStepDto DTO for pipeline steps in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::pipeline
{
    /**
     * @class PipelineStepDto
     * @brief Data transfer object for a single pipeline step.
     *
     * Contains information about the class to instantiate, its alias, constructor arguments,
     * the function to call, and its arguments.
     */
    class PipelineStepDto final : public oatpp::DTO
    {
        DTO_INIT(PipelineStepDto, DTO)

        /**
         * @brief Name of the class to instantiate.
         */
        DTO_FIELD(oatpp::String, class_name, "create");

        /**
         * @brief Alias for the created instance.
         */
        DTO_FIELD(oatpp::String, instance_alias, "as");

        /**
         * @brief Arguments for the class constructor.
         */
        DTO_FIELD(oatpp::List<oatpp::Any>, constructor_args, "with");

        /**
         * @brief Name of the function to call on the instance.
         */
        DTO_FIELD(oatpp::String, function_name, "call");

        /**
         * @brief Arguments for the function call.
         */
        DTO_FIELD(oatpp::List<oatpp::Any>, function_args, "using");
    };
} // namespace engine::network::dto::pipeline

#include OATPP_CODEGEN_END(DTO)
