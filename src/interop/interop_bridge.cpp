#include "interop/interop_bridge.hpp"
#include "bindings/lua/registrar.hpp"
#include "execution/execution_engine.hpp"
#include "execution/execution_type.hpp"

namespace interop {
    std::once_flag bindings_flag;

    void init_bindings()
    {
        try {
            bindings::lua::Registrar::instance().register_bindings();
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to register Lua bindings: " << e.what()
                      << std::endl;
        }
    }

    bool execute_script(const fs::path& script_path,
                        const fs::path& input_data_path,
                        const fs::path& output_data_path)
    {
        std::call_once(bindings_flag, init_bindings);

        (void)input_data_path;
        (void)output_data_path;

        try {
            const execution::ExecutionEngine exec_engine(
                    execution::ExecutionType::Lua_Script);
            exec_engine.execute_script_file(script_path);
            return true;
        }
        catch (...) {
            return false;
        }
    }
} // namespace interop
