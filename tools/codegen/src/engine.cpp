#include "engine.hpp"
#include "clang_analyzer.hpp"
#include "header_collector.hpp"
#include "sol2_generator.hpp"
#include "yaml_reader.hpp"

namespace codegen {

    std::once_flag Engine::m_flag;

    void Engine::generate_lua_bindings() const
    {
        std::call_once(m_flag, [this] {
            HeaderCollector collector;
            collector.collect_headers(m_include_dir, k_dummy_cpp);

            YamlReader reader;
            reader.load_file(m_config_yaml);

            ClangAnalyzer analyzer(k_dummy_cpp, reader.classes(), reader.free_functions());

            Sol2Generator generator(k_generated_lua_bindings);
            generator.generate(analyzer.found_includes(), analyzer.found_classes(), analyzer.found_free_functions());
        });
    }

} // namespace codegen
