#pragma once

#include "types/execution_request.hpp"
#include "types/execution_response.hpp"

namespace engine::interop {

    bool initialize_bindings();

    types::ExecutionResponse execute_script(const types::ExecutionRequest* request);

} // namespace engine::interop
