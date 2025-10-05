#include "interop/interop_bridge.hpp"

#include "bindings/lua/registry.hpp"
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

    std::unique_ptr<types::ExecutionResponse> execute_script(const types::ExecutionRequest* request)
    {
        try
        {
            auto& engine = execution::ExecutionEngine::instance();

            if (request->type == types::ExecutionType::Lua_Script)
            {
                engine.set_execution_type(request->type);
                return engine.execute_script(request->script_path);
            }

            return utils::ResponseFactory::create_error(types::ExecutionStatus::Failure,
                                                        types::ExecutionErrorType::Invalid_Argument,
                                                        "Unsupported execution type");
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to execute script: " << e.what() << std::endl;
            return utils::ResponseFactory::create_error(types::ExecutionStatus::Failure,
                                                        types::ExecutionErrorType::Execution_Failed, e.what());
        }
        catch (...)
        {
            std::cerr << "Failed to execute script: unknown exception" << std::endl;
            return utils::ResponseFactory::create_error(
                types::ExecutionStatus::Failure, types::ExecutionErrorType::Execution_Failed, "Unknown error occurred");
        }
    }

} // namespace engine::interop
