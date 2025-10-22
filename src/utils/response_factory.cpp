#include "utils/response_factory.hpp"

#include "interop/types/execution_error_type.h"
#include "interop/types/execution_response.h"

#if defined(_WIN32)
    #define strdup _strdup
#endif

namespace engine::utils
{
    void ExecutionResponseDeleter::operator()(interop::types::ExecutionResponse* ptr) const
    {
        if (!ptr)
        {
            return;
        }

        if (ptr->error.message)
        {
            free(ptr->error.message);
        }

        delete ptr;
    }

    ExecutionResponsePtr ResponseFactory::create_error(interop::types::ExecutionStatus status,
                                                       interop::types::ExecutionErrorType type,
                                                       const std::string& message)
    {
        ExecutionResponsePtr response(new interop::types::ExecutionResponse(), ExecutionResponseDeleter());

        response->status = status;
        response->error.type = type;
        response->error.message = strdup(message.c_str());

        return response;
    }

    ExecutionResponsePtr ResponseFactory::create_success(interop::types::ExecutionMetadata metadata)
    {
        ExecutionResponsePtr response(new interop::types::ExecutionResponse(), ExecutionResponseDeleter());

        response->status = interop::types::ExecutionStatus::Success;
        response->metadata = metadata;

        response->error.type = {};
        response->error.message = nullptr;

        return response;
    }
} // namespace engine::utils
