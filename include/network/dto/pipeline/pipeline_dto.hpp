/**
 * Project: Engine
 * File: pipeline_dto.hpp
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
 * @file pipeline_dto.hpp
 * @brief Defines the PipelineDto DTO for pipeline tasks in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "pipeline_step_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::pipeline
{
    /**
     * @class PipelineDto
     * @brief Data transfer object for pipeline tasks.
     *
     * Contains the pipeline task identifier and a list of pipeline steps.
     */
    class PipelineDto final : public oatpp::DTO
    {
        DTO_INIT(PipelineDto, DTO)

        /**
         * @brief Identifier for the pipeline task.
         */
        DTO_FIELD(oatpp::String, pipeline_task, "task");

        /**
         * @brief
         * @return List of steps in the pipeline.
         */
        DTO_FIELD(oatpp::List<oatpp::Object<PipelineStepDto>>, pipeline_steps, "steps");
    };
} // namespace engine::network::dto::pipeline

#include OATPP_CODEGEN_END(DTO)
