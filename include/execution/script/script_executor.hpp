#pragma once

#include "interop/types/execution_response.hpp"

namespace execution::script {

    class ScriptExecutor final {
    public:
        ScriptExecutor() = default;

        static interop::types::ExecutionResponse execute_from_file(const fs::path& script_path);
        static interop::types::ExecutionResponse execute_from_string(const std::string& script_content);

    private:
        static fs::path normalize_path(const fs::path& path);
    };

} // namespace execution::script
