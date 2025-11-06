/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
