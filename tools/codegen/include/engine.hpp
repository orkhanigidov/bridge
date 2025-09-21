#pragma once

namespace codegen {

    class Engine final {
    public:
        explicit Engine(const std::string& include_dir, const std::string& config_yaml)
            : m_include_dir(include_dir), m_config_yaml(config_yaml) {}

        void generate_lua_bindings() const;

    private:
        static constexpr auto k_dummy_cpp = "dummy.cpp";
        static constexpr auto k_generated_lua_bindings = "generated_bindings.cpp";

        std::string m_include_dir;
        std::string m_config_yaml;
        static std::once_flag m_flag;
    };

} // namespace codegen
