#pragma once

#include "execution/pipeline/pipeline_executor.hpp"
#include "interop/types/execution_response.hpp"
#include "interop/types/execution_type.hpp"

namespace engine::execution {

    class ExecutionEngine final {
    public:
        static ExecutionEngine& instance()
        {
            static ExecutionEngine instance;
            return instance;
        }

        std::unique_ptr<interop::types::ExecutionResponse> execute_script(const std::string &script) const;
        std::unique_ptr<interop::types::ExecutionResponse> execute_script_file(const fs::path& path) const;
        interop::types::ExecutionResponse execute_pipeline(const std::string& json) const;
        interop::types::ExecutionResponse execute_pipeline_file(const fs::path& path) const;

        void set_execution_type(interop::types::ExecutionType execution_type) noexcept {
            m_execution_type = execution_type;
        }

    private:
        ExecutionEngine() = default;

        interop::types::ExecutionType m_execution_type{interop::types::ExecutionType::Lua_Script};

        static bool is_valid_path(const fs::path& path);
    };

} // namespace engine::execution
