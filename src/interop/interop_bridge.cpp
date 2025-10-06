#include "interop/interop_bridge.hpp"

#include "bindings/lua/lua_binder.hpp"
#include "execution/execution_engine.hpp"
#include "utils/response_factory.hpp"

namespace engine::interop
{

    static std::once_flag lua_bindings_init_flag;

    bool initialize_bindings()
    {
        try
        {
            std::call_once(lua_bindings_init_flag, [] { bindings::lua::Registry::instance().register_bindings(); });

            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to initialize Lua bindings: " << e.what() << std::endl;
            return false;
        }
        catch (...)
        {
            std::cerr << "Failed to initialize Lua bindings: unknown exception" << std::endl;
            return false;
        }
    }

    utils::ExecutionResponsePtr execute(const types::ExecutionRequest* request)
    {
        try
        {
            execution::ExecutionEngine execution;
            return execution.execute_lua(types::Lua_Script, request->script_or_path);
        }
        catch (const std::exception& e)
        {
            return utils::ResponseFactory::create_error(types::ExecutionStatus::Failure,
                                                        types::ExecutionErrorType::Execution_Failed, e.what());
        }
        catch (...)
        {
            return utils::ResponseFactory::create_error(
                types::ExecutionStatus::Failure, types::ExecutionErrorType::Execution_Failed, "Unknown error occurred");
        }
    }

} // namespace engine::interop
