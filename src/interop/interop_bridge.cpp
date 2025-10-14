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
            std::call_once(lua_bindings_init_flag, []
            {
                sol::state lua;
                bindings::lua::LuaBinder binder;
                binder.register_bindings(lua);
            });
            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to initialize Lua bindings: " << e.what() << std::endl;
            return false;
        }
    }

    utils::ExecutionResponsePtr execute(const types::ExecutionRequest* request)
    {
        try
        {
            execution::ExecutionEngine execution;

            switch (request->type)
            {
            case types::ExecutionType::Lua_Script:
                return execution.execute(request->type, request->script);
            default:
                return utils::ResponseFactory::create_error(types::ExecutionStatus::Failure,
                                                            types::ExecutionErrorType::Invalid_Argument,
                                                            "Unsupported execution type");
            }
        }
        catch (const std::exception& e)
        {
            return utils::ResponseFactory::create_error(types::ExecutionStatus::Failure,
                                                        types::ExecutionErrorType::Execution_Failed,
                                                        e.what());
        }
    }
} // namespace engine::interop
