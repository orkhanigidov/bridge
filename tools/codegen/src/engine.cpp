/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file engine.cpp
 * @brief Implements the Engine class for generating Lua bindings.
 */

#include "engine.hpp"

#include <exception>
#include <filesystem>

#include "analysis/analysis_data.hpp"
#include "analysis/clang_analyzer.hpp"
#include "generation/sol2_generator.hpp"
#include "io/header_collector.hpp"
#include "io/yaml_reader.hpp"

namespace codegen
{
    /**
     * @brief Generates Lua bindings based on the provided configuration and directories.
     *
     * This method performs the following steps:
     * 1. Collects header files from the include and wrapper directories.
     * 2. Reads and parses the YAML configuration file.
     * 3. Analyzes the code using Clang.
     * 4. Generates Lua bindings using the Sol2 generator.
     *
     * Progress is logged via the log method. Throws EngineException on failure.
     *
     * @throws EngineException if any stage of the code generation fails.
     */
    void Engine::generate_lua_bindings() const
    {
        try
        {
            const auto dummy_cpp_path = output_dir_ / DUMMY_CPP_FILENAME;
            const auto generated_bindings_path = output_dir_ / GENERATED_BINDINGS_FILENAME;

            log("Stage 1: Collecting headers...");
            io::HeaderCollector::collect_headers_to_file({include_dir_, wrapper_dir_}, dummy_cpp_path);

            log("Stage 1.1: Collecting manual binding headers...");
            std::vector<std::pair<std::string, std::string>> manual_bindings;
            if (std::filesystem::exists(manual_register_dir_) && std::filesystem::is_directory(manual_register_dir_))
            {
                for (const auto& entry : std::filesystem::directory_iterator(manual_register_dir_))
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".hpp" && entry.path().filename().string().rfind("manual_", 0) == 0)
                    {
                        std::string filename_stem = entry.path().stem().string();
                        std::string function_name = "register_" + filename_stem;

                        std::string relative_path = manual_register_dir_.stem().string() + "/" + entry.path().filename().string();

                        manual_bindings.emplace_back(std::move(relative_path), std::move(function_name));
                        log("    Found manual bindings: " + entry.path().filename().string() + " (will call " + function_name + ")");
                    }
                }
            }
            else
            {
                log("    Warning: Manual binding directory not found, skipping: " + manual_register_dir_.string());
            }

            log("Stage 2: Reading YAML configuration...");
            const auto reader = io::YamlReader::from_file(config_yaml_);

            log("Stage 3: Analyzing code with Clang...");
            const analysis::AnalysisConfig config = {
                include_dir_.string(),
                wrapper_dir_.string(),
                reader.classes(),
                reader.free_functions(),
                reader.enums()
            };
            analysis::ClangAnalyzer analyzer(dummy_cpp_path, config);

            log("Stage 4: Generating Lua bindings...");
            generation::Sol2Generator generator(generated_bindings_path);
            generator.generate(analyzer.found_includes(),
                               analyzer.found_namespaces(),
                               analyzer.found_containers(),
                               analyzer.found_classes(),
                               analyzer.found_free_functions(),
                               analyzer.found_enums(),
                               manual_bindings);

            log("Lua bindings generated successfully! -> " + generated_bindings_path.string());
        } catch (const std::exception& e)
        {
            throw EngineException("Code generation failed: " + std::string(e.what()));
        }
    }
} // namespace codegen
