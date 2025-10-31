/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#include "engine.hpp"

#include <exception>
#include <filesystem>
#include <format>

#include "analysis/analysis_data.hpp"
#include "analysis/clang_analyzer.hpp"
#include "generation/sol2_generator.hpp"
#include "io/header_collector.hpp"
#include "io/yaml_reader.hpp"

namespace codegen
{
    void Engine::generate_lua_bindings() const
    {
        try
        {
            const auto dummy_cpp_path = output_dir_ / DUMMY_CPP_FILENAME;
            const auto generated_bindings_path = output_dir_ / GENERATED_BINDINGS_FILENAME;

            log("Stage 1: Collecting headers...");
            io::HeaderCollector::collect_headers_to_file({include_dir_, wrapper_dir_}, dummy_cpp_path);

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
                               analyzer.found_classes(),
                               analyzer.found_free_functions(),
                               analyzer.found_enums());

            log(std::format("Lua bindings generated successfully! -> {}", generated_bindings_path.string()));
        } catch (const std::exception& e)
        {
            throw EngineException(std::format("Code generation failed: {}", e.what()));
        }
    }
} // namespace codegen
