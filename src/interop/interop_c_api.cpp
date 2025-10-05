#include "interop/interop_c_api.hpp"

#include "interop/interop_bridge.hpp"
#include "utils/response_factory.hpp"

bool InitializeBindings()
{
    return engine::interop::initialize_bindings();
}

engine::interop::types::ExecutionResponse* ExecuteScript(const engine::interop::types::ExecutionRequest* request)
{
    if (!request)
    {
        return engine::utils::ResponseFactory::create_error(
                   engine::interop::types::ExecutionStatus::Failure,
                   engine::interop::types::ExecutionErrorType::Invalid_Argument, "The provided request is null.")
            .release();
    }

    return engine::interop::execute(request).release();
}

void FreeExecutionResponse(engine::interop::types::ExecutionResponse* response)
{
    if (!response)
    {
        return;
    }

    if (response->error.message)
    {
        free(response->error.message);
    }

    delete response;
}
