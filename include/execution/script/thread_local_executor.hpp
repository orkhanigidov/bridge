#pragma once

#include "script_executor.hpp"

namespace engine::execution::script
{
    inline ScriptExecutor& get_thread_local_executor()
    {
        thread_local ScriptExecutor executor;
        return executor;
    }
} // namespace engine::execution::script
