/**
 * Project: Engine
 * File: reserved_keywords.hpp
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
 * @file reserved_keywords.hpp
 * @brief Defines reserved keyword constants for execution input and output paths.
 */

#pragma once

namespace engine::execution::reserved
{
    /**
     * @brief Reserved keyword for the input path.
     */
    inline constexpr auto InputPath = "__input__";

    /**
     * @brief Reserved keyword for the output path.
     */
    inline constexpr auto OutputPath = "__output__";
} // namespace engine::execution::reserved
