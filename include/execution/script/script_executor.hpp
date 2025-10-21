#pragma once

#include "utils/response_factory.hpp"

#include <sol/state.hpp>

namespace engine::execution::script
{
    /**
     * @brief
     */
    class ScriptExecutor final
    {
    public:
        /**
         * @brief Default constructor
         */
        ScriptExecutor();

        /**
         * @brief
         * @param script_path
         * @return
         */
        utils::ExecutionResponsePtr execute_from_file(const fs::path& script_path);

        /**
         * @brief
         * @param script_content
         * @return
         */
        utils::ExecutionResponsePtr execute_from_string(const std::string& script_content);

    private:
        sol::state lua_;
    };
} // namespace engine::execution::script
