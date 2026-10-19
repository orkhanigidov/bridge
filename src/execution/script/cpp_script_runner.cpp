/**
 * Project: Engine
 * File: cpp_script_runner.cpp
 *
 * Copyright (C) 2026 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.
 */

/**
 * @file cpp_script_runner.cpp
 * @brief Implements the CppScriptRunner utility for compiling and running C++ scripts.
 */

#include "execution/script/cpp_script_runner.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

namespace engine::execution::script
{
    CoreExecutionResult CppScriptRunner::run_from_string(const ScriptContext& context) const
    {
        try
        {
            namespace fs = std::filesystem;
            auto temp_dir = fs::temp_directory_path() / "ogdf_engine_cpp_runs";
            fs::create_directories(temp_dir);

            std::string session_id = std::to_string(std::random_device{}());
            fs::path source_path = temp_dir / ("source_" + session_id + ".cpp");
            fs::path binary_path = temp_dir / ("binary_" + session_id);

            if (!context.input_path || !context.output_path)
            {
                return {CoreExecutionStatus::Failure, {CoreExecutionErrorType::ExecutionFailed, "Input or output path is missing"}};
            }

            std::ofstream source_file(source_path);
            if (!source_file)
            {
                return {CoreExecutionStatus::Failure, {CoreExecutionErrorType::ExecutionFailed, "Cannot write temporary source file"}};
            }
            source_file << context.script_content;
            source_file.close();

            // TODO: Change static paths to dynamic ones based on CMake configuration or environment variables.
            std::ostringstream compile_cmd;
            compile_cmd << "g++ -O3 -std=c++17"
                        << " -I/home/dev/app/build/_deps/ogdf-src/include"
                        << " -I/home/dev/app/build/_deps/ogdf-build/include"
                        << " " << source_path.string()
                        << " -o " << binary_path.string()
                        << " -L/home/dev/app/build/_deps/ogdf-build"
                        << " -lOGDF"
                        << " -lCOIN";

            int compile_res = std::system(compile_cmd.str().c_str());
            if (compile_res != 0)
            {
                return {CoreExecutionStatus::Failure, {CoreExecutionErrorType::ExecutionFailed, "C++ Compilation failed"}};
            }

            // Run from the script's directory so literal file names resolve against
            // its own files. The output path may be a file; use its parent then.
            const fs::path run_directory = fs::is_directory(*context.output_path)
                ? *context.output_path
                : context.output_path->parent_path();

            const int run_res = std::system((std::string("cd \"") + run_directory.string() + "\" && \"" + binary_path.string() + "\"").c_str());
            if (run_res != 0)
            {
                return {CoreExecutionStatus::Failure, {CoreExecutionErrorType::ExecutionFailed, "C++ Execution failed"}};
            }

            return {CoreExecutionStatus::Success};
        }
        catch (const std::exception& e)
        {
            return {CoreExecutionStatus::Failure, {CoreExecutionErrorType::ExecutionFailed, e.what()}};
        }
    }
} // namespace engine::execution::script
