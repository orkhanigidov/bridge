#pragma once

#include "types/execution_request.h"
#include "utils/response_factory.hpp"

namespace engine::interop
{

    bool initialize_bindings();

    utils::ExecutionResponsePtr execute(const types::ExecutionRequest* request);

} // namespace engine::interop
