#include "interop/types/execution_error_type.hpp"
#include "interop/types/execution_response.hpp"

#if defined(_WIN32) && !defined(__clang__)
    #define strdup _strdup
#endif

namespace engine::utils
{

    namespace ResponseFactory
    {

        std::unique_ptr<interop::types::ExecutionResponse> create_error(interop::types::ExecutionStatus status, interop::types::ExecutionErrorType type, const char* message)
        {
            auto response = std::make_unique<interop::types::ExecutionResponse>();

            response->status = status;
            response->error.type = type;

            if (message)
            {
                response->error.message = strdup(message);
            }
            else
            {
                response->error.message = nullptr;
            }

            return response;
        }

        std::unique_ptr<interop::types::ExecutionResponse> create_success(uint64_t duration_milliseconds)
        {
            auto response = std::make_unique<interop::types::ExecutionResponse>();

            response->status = interop::types::ExecutionStatus::Success;
            response->metadata.duration_milliseconds = duration_milliseconds;

            return response;
        }

    }

} // namespace engine::utils