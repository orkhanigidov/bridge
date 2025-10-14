#pragma once

namespace engine::network
{
    enum class ServerState
    {
        STOPPED,
        INITIALIZED,
        RUNNING,
        SHUTTING_DOWN
    };
} // namespace engine::network
