#include "engine/bridge/native_bridge.hpp"

#include "pch.hpp"

namespace engine::bridge
{
    void native_bridge::process_execution_request(std::string_view data) {}
} // namespace engine::bridge

extern "C"
{
    void run_native_script(const char* json)
    {
        engine::bridge::native_bridge bridge;
        bridge.process_execution_request(json);
    }
}