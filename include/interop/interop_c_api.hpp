#pragma once

#include "engine_export.hpp"
#include "types/execution_request.hpp"
#include "types/execution_response.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    ENGINE_API bool InitializeBindings();

    ENGINE_API engine::interop::types::ExecutionResponse ExecuteScript(const engine::interop::types::ExecutionRequest* request);

#ifdef __cplusplus
} // extern "C"
#endif
