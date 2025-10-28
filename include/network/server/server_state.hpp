#pragma once

namespace engine::network::server
{
    enum class ServerState
    {
        Stopped,
        Initialized,
        Running,
        ShuttingDown
    };
} // namespace engine::network::server
