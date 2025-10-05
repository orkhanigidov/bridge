#pragma once

#include "utils/response_factory.hpp"

namespace engine::execution::script
{

    class ScriptExecutor final
    {
      public:
        ScriptExecutor() = delete;

        static utils::ExecutionResponsePtr execute_from_file(const fs::path& script_path);
        static utils::ExecutionResponsePtr execute_from_string(const std::string& script_content);

      private:
        static fs::path normalize_path(const fs::path& path);
    };

} // namespace engine::execution::script
