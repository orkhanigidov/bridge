#pragma once

namespace engine::network::server
{
    enum class ServerState
    {
        STOPPED,
        INITIALIZED,
        RUNNING,
        SHUTTING_DOWN
    };
} // namespace engine::network::server
