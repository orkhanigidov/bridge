/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file type_dto.hpp
 * @brief Defines the ExecutionTypeDto enum DTO for execution types in the network API.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    /**
     * @enum ExecutionTypeDto
     * @brief Enumerates execution types for network requests.
     *
     * - LuaScript: Represents a Lua script execution.
     * - Pipeline: Represents a pipeline execution.
     */
    ENUM(ExecutionTypeDto, v_int8,
         VALUE(LuaScript, 0, "LuaScript"),
         VALUE(Pipeline, 1, "Pipeline")
    )
} // namespace engine::network::dto::execution
