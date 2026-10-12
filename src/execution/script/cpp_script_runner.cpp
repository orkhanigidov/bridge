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
#include <regex>
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

            std::string preprocessed_source = preprocess_source(context.script_content, *context.input_path, *context.output_path);

            std::ofstream source_file(source_path);
            if (!source_file)
            {
                return {CoreExecutionStatus::Failure, {CoreExecutionErrorType::ExecutionFailed, "Cannot write temporary source file"}};
            }
            source_file << preprocessed_source;
            source_file.close();
            
            std::ostringstream compile_cmd;
            compile_cmd << "g++ -O3 " << source_path.string() << " -o " << binary_path.string() << " -logdf";

            int compile_res = std::system(compile_cmd.str().c_str());
            if (compile_res != 0)
            {
                return {CoreExecutionStatus::Failure, {CoreExecutionErrorType::ExecutionFailed, "C++ Compilation failed"}};
            }

            // Execution command
            int run_res = std::system(binary_path.string().c_str());
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

    std::string CppScriptRunner::preprocess_source(const std::string& source, 
                                                 const std::filesystem::path& input_path, 
                                                 const std::filesystem::path& output_path) const
    {
        std::string modified = source;

        // Replace GraphIO::read(G, "filename") with GraphIO::read(G, "actual_path")
        std::regex read_regex(R"(GraphIO::read\s*\(\s*G\s*,\s*"[^"]*"\s*\))");
        std::string read_replacement = "GraphIO::read(G, \"" + input_path.string() + "\")";
        modified = std::regex_replace(modified, read_regex, read_replacement);

        // Replace GraphIO::write(..., "filename", ...) with GraphIO::write(..., "actual_path", ...)
        // We target the SVG output as per requirements.
        std::regex write_svg_regex(R"(GraphIO::write\s*\(\s*GA\s*,\s*"[^"]*"\s*,\s*GraphIO::drawSVG\s*\))");
        std::string write_replacement = "GraphIO::write(GA, \"" + output_path.string() + "\", GraphIO::drawSVG)";
        modified = std::regex_replace(modified, write_svg_regex, write_replacement);

        return modified;
    }
} // namespace engine::execution::script
