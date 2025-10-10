#include "engine.hpp"
#include "analysis/clang_analyzer.hpp"
#include "analysis/header_collector.hpp"
#include "generation/sol2_generator.hpp"
#include "io/yaml_reader.hpp"

namespace codegen
{
    std::once_flag Engine::flag_;

    void Engine::generate_lua_bindings() const
    {
        std::call_once(flag_, [this]
        {
            analysis::HeaderCollector collector;
            collector.collect_headers(include_dir_, k_dummy_cpp);

            io::YamlReader reader;
            reader.load_file(config_yaml_);

            analysis::ClangAnalyzer analyzer(k_dummy_cpp, reader.classes(), reader.free_functions());

            generation::Sol2Generator generator(k_generated_lua_bindings);
            generator.generate(analyzer.found_includes(), analyzer.found_classes(), analyzer.found_free_functions());
        });
    }
} // namespace codegen
