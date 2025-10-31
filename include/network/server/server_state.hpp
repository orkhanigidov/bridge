/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

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
