#include "interop/interop_c_api.hpp"

#include "interop/interop_bridge.hpp"
#include "utils/response_factory.hpp"

bool InitializeBindings()
{
    return engine::interop::initialize_bindings();
}

ExecutionResponse* ExecuteScript(const ExecutionRequest* request)
{
    if (!request)
    {
        return engine::utils::ResponseFactory::create_error(engine::interop::types::ExecutionStatus::Failure,
                                                            engine::interop::types::ExecutionErrorType::Invalid_Argument,
                                                            "The provided request is null.").release();
    }

    return engine::interop::execute(request).release();
}

void FreeExecutionResponse(ExecutionResponse* response)
{
    if (!response)
    {
        return;
    }

    engine::utils::ExecutionResponseDeleter deleter;
    deleter(response);
}
