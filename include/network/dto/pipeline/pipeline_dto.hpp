/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
