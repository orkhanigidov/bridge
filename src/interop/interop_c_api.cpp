#include "interop/interop_c_api.hpp"

#include "interop/interop_bridge.hpp"

bool InitializeBindings() {
    return engine::interop::initialize_bindings();
}

engine::interop::types::ExecutionResponse* ExecuteScript(const engine::interop::types::ExecutionRequest* request) {
    if (!request) {
        return new engine::interop::types::ExecutionResponse(engine::interop::types::ExecutionStatus::Failure);
    }
    
    return engine::interop::execute_script(request).release();
}

void FreeExecutionResponse(engine::interop::types::ExecutionResponse* response) {
    if (!response) {
        return;
    }

    if (response->error.message)
    {
        free(response->error.message);
    }

    free(response);
}
