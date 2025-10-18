#pragma once

namespace codegen
{
    class EngineException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class Engine final
    {
    public:
        explicit Engine(fs::path include_dir, fs::path config_yaml): include_dir_(std::move(include_dir)),
                                                                     config_yaml_(std::move(config_yaml))
        {
            wrapper_dir_ = fs::path(fs::absolute(WRAPPER_INCLUDE_PATH).generic_string());
        }

        void generate_lua_bindings() const;

    private:
        static constexpr auto DUMMY_CPP = "dummy.cpp";
        static constexpr auto GENERATED_LUA_BINDINGS = "generated_bindings.cpp";
        static constexpr auto WRAPPER_INCLUDE_PATH = "../tools/codegen/include/wrappers";

        fs::path include_dir_;
        fs::path wrapper_dir_;
        fs::path config_yaml_;
    };
} // namespace codegen
