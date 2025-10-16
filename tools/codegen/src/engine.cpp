#include "engine.hpp"

#include "analysis/clang_analyzer.hpp"
#include "io/header_collector.hpp"
#include "io/yaml_reader.hpp"
#include "generation/sol2_generator.hpp"

namespace codegen
{
    void
    Engine::generate_lua_bindings() const
    {
        try
        {
            std::cout << "Stage 1: Collecting headers..." << std::endl;
            io::HeaderCollector::collect_headers_to_file(include_dir_, DUMMY_CPP);

            std::cout << "Stage 2: Reading YAML configuration..." << std::endl;
            const auto reader = io::YamlReader::from_file(config_yaml_.string());

            std::cout << "Stage 3: Analyzing code with Clang..." << std::endl;
            const analysis::AnalysisConfig config = {include_dir_.string(), reader.classes(), reader.free_functions()};
            analysis::ClangAnalyzer analyzer(DUMMY_CPP, config);

            std::cout << "Stage 4: Generating Lua bindings..." << std::endl;
            generation::Sol2Generator generator(GENERATED_LUA_BINDINGS);
            generator.generate(analyzer.found_includes(), analyzer.found_classes(), analyzer.found_free_functions(), analyzer.found_enums());

            std::cout << "Lua bindings generated successfully!" << std::endl;
        }
        catch (const std::exception& e)
        {
            throw EngineException("Code generation failed: " + std::string(e.what()));
        }
    }
} // namespace codegen
