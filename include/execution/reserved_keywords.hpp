/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
