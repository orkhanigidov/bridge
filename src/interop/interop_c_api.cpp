#pragma once

#include "interop/interop_bridge.hpp"

bool InitializeBindings() {
    return engine::interop::initialize_bindings();
}

engine::interop::types::ExecutionResponse ExecuteScript(const engine::interop::types::ExecutionRequest* request) {
    if (!request) {
        return {};
    }
    
    return engine::interop::execute_script(request);
}
