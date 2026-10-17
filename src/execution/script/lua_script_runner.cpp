/**
 * Project: Engine
 * File: lua_script_runner.cpp
 *
 * Copyright (C) 2025-2026 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file lua_script_runner.cpp
 * @brief Implements the LuaScriptRunner utility for running Lua scripts with a context.
 */

#include "execution/script/lua_script_runner.hpp"

#include <filesystem>
#include <mutex>
#include <random>

#include <ogdf/basic/basic.h>

#include "execution/core_execution_result.hpp"
#include "execution/reserved_keywords.hpp"
#include "execution/script/script_executor.hpp"
#include "utils/filesystem_utils.hpp"

namespace engine::execution::script
{
    namespace
    {
        /**
         * @brief Serializes access to OGDF's process-global random number generator.
         *
         * OGDF elderberry-202309 keeps a single static std::mt19937
         * (src/ogdf/basic/basic.cpp) shared by all threads, while oatpp's
         * HttpConnectionHandler handles requests on separate threads. The whole
         * script execution must hold this lock so the "seed -> layout" sequence
         * is atomic.
         */
        std::mutex &rng_mutex()
        {
            static std::mutex mutex;
            return mutex;
        }

        /**
         * @brief The initial RNG state of a freshly started process.
         *
         * The C++ runner executes each script in a fresh subprocess, where OGDF's
         * global generator is a default-constructed std::mt19937 (basic.cpp:
         * "static std::mt19937 s_random;"). Resetting the Lua path to the same
         * default seed before every run reproduces that state exactly.
         */
        constexpr int g_default_mt19937_seed = static_cast<int>(std::mt19937::default_seed);
    } // namespace

    /**
     * @brief Creates a Lua environment for the script using the provided context. Sets input and output paths as environment variables if present.
     * @param context The script context.
     * @return The created Lua environment.
     */
    sol::environment LuaScriptRunner::create_script_env(const ScriptContext& context) const
    {
        sol::environment env(lua_, sol::create, lua_.globals());

        if (context.input_path)
        {
            env[reserved::InputPath] = utils::filesystem::to_forward_slashes(*context.input_path);
        }

        if (context.output_path)
        {
            env[reserved::OutputPath] = utils::filesystem::to_forward_slashes(*context.output_path);
        }

        return env;
    }

    /**
     * @brief Runs a Lua script from a string using the provided context.
     * @param context The script context containing script content and optional paths.
     * @return The result of the script execution.
     */
    CoreExecutionResult LuaScriptRunner::run_from_string(const ScriptContext& context) const
    {
        std::lock_guard<std::mutex> rng_lock(rng_mutex());
        ogdf::setSeed(g_default_mt19937_seed);

        const sol::environment env = create_script_env(context);
        const ScriptExecutor executor(lua_);
        return executor.execute_from_string(context.script_content, env);
    }

    /**
     * @brief Runs a Lua script from a file using the provided context. Returns a failure result if the script file does not exist.
     * @param context The script context containing script content and optional paths.
     * @return The result of the script execution.
     */
    CoreExecutionResult LuaScriptRunner::run_from_file(const ScriptContext& context) const
    {
        if (!std::filesystem::exists(context.script_content))
        {
            return {
                CoreExecutionStatus::Failure,
                {
                    CoreExecutionErrorType::FileNotFound,
                    "Script file not found"
                }
            };
        }

        std::lock_guard<std::mutex> rng_lock(rng_mutex());
        ogdf::setSeed(g_default_mt19937_seed);

        const sol::environment env = create_script_env(context);
        const ScriptExecutor executor(lua_);
        return executor.execute_from_file(context.script_content, env);
    }
} // namespace engine::execution::script
