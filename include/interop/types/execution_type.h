/**
 * Project: Engine
 * File: execution_type.h
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
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
