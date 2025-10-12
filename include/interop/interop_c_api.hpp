#pragma once

#include "engine_export.hpp"
#include "types/execution_request.h"
#include "types/execution_response.h"

typedef engine::interop::types::ExecutionRequest ExecutionRequest;
typedef engine::interop::types::ExecutionResponse ExecutionResponse;

#ifdef __cplusplus
extern "C" {
#endif

ENGINE_API bool InitializeBindings();

ENGINE_API ExecutionResponse*ExecuteScript(const ExecutionRequest* request);

ENGINE_API void FreeExecutionResponse(ExecutionResponse* response);

#ifdef __cplusplus
} // extern "C"
#endif
