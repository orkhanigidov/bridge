#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{

    typedef enum ExecutionStatus
    {
        Success = 0,
        Failure = 1,
        Timeout = 2
    } ExecutionStatus;

} // namespace engine::interop::types

#pragma pack(pop)