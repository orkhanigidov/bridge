/**
 * Project: Engine
 * File: interop_bridge.hpp
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
 * @file interop_bridge.hpp
 * @brief Declares the interop bridge interface for execution requests and thread state management.
 */

#pragma once

#include "types/execution_request.h"
#include "utils/response_factory.hpp"

namespace engine::interop
{
    /**
     * @brief Prewarms the thread state for execution.
     * @return True if the thread state was successfully prewarmed, false otherwise.
     */
    bool prewarm_thread_state();

    /**
     * @brief Executes a request in the engine interop layer.
     * @param request Pointer to the execution request.
     * @return A pointer to the execution response.
     */
    utils::ExecutionResponsePtr execute(const types::ExecutionRequest* request);
} // namespace engine::interop
