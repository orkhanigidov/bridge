#pragma once

#include "engine_export.hpp"
#include "types/execution_request.h"
#include "types/execution_response.h"

#ifdef __cplusplus
extern "C"
{
#endif

    ENGINE_API bool InitializeBindings();

    ENGINE_API engine::interop::types::ExecutionResponse* ExecuteScript(const engine::interop::types::ExecutionRequest* request);

    ENGINE_API void FreeExecutionResponse(engine::interop::types::ExecutionResponse* response);

#ifdef __cplusplus
} // extern "C"
#endif
