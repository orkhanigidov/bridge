#include "engine/native/native_bridge.hpp"

#include "engine/execution/execution_engine.hpp"
#include "engine/bindings/lua/registrar.hpp"

namespace engine::native
{
    NativeBridge::NativeBridge()
    {
        try
        {
            bindings::lua::Registrar::instance().register_bindings();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to register Lua bindings: " << e.what() << std::endl;
        }
    }

    bool NativeBridge::execute_script(const fs::path& script_path)
    {
        try
        {
            execution::ExecutionEngine exec_engine;
            exec_engine.execute_script_file(script_path);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
} // namespace engine::native
