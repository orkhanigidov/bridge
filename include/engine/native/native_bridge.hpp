#pragma once

namespace engine::native
{
    class NativeBridge final
    {
    public:
        static NativeBridge& instance()
        {
            static NativeBridge bridge;
            return bridge;
        }

        static bool execute_script(const fs::path& script_path);

    private:
        NativeBridge();
    };
} // namespace engine::native
