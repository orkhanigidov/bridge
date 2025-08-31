#pragma once

#include "interop/interop_bridge.hpp"

bool InitializeBindings() {
    return interop::initialize_bindings();
}

interop::types::ExecutionResponse ExecuteScript(const interop::types::ExecutionRequest* request) {
    if (!request) {
        return {};
    }
    
    return interop::execute_script(request);
}
