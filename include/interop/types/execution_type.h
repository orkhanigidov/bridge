/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file execution_type.h
 * @brief Defines the ExecutionType enum for the engine interop layer.
 */

#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    /**
     * @enum ExecutionType
     * @brief Represents the type of execution in the engine interop layer.
     *
     * Used to specify whether the execution is a Lua script, a Lua file, or a pipeline.
     */
    typedef enum ExecutionType
    {
        /**
         * @brief The execution is a Lua script.
         */
        LuaScript = 0,

        /**
         * @brief The execution is a Lua file.
         */
        LuaFile = 1,

        /**
         * @brief The execution is a pipeline.
         */
        Pipeline = 2,
    } ExecutionType;
} // namespace engine::interop::types

#pragma pack(pop)
