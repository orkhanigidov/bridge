#pragma once

namespace codegen {

    class Engine final {
    public:
        explicit Engine(const std::string& include_dir, const std::string& config_yaml)
            : include_dir_(include_dir), config_yaml_(config_yaml) {}

        void generate_lua_bindings() const;

    private:
        static constexpr auto k_dummy_cpp = "dummy.cpp";
        static constexpr auto k_generated_lua_bindings = "generated_bindings.cpp";

        std::string include_dir_;
        std::string config_yaml_;
        static std::once_flag flag_;
    };

} // namespace codegen
