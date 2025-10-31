/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include "types/execution_request.h"
#include "utils/response_factory.hpp"

namespace engine::interop
{
    bool prewarm_thread_state();

    utils::ExecutionResponsePtr execute(const types::ExecutionRequest* request);
} // namespace engine::interop
