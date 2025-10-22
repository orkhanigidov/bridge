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
        explicit Engine(fs::path include_dir, fs::path wrapper_dir, fs::path config_yaml,
                        fs::path output_dir, std::ostream* logger): include_dir_(std::move(include_dir)),
                                                                    wrapper_dir_(std::move(wrapper_dir)),
                                                                    config_yaml_(std::move(config_yaml)),
                                                                    output_dir_(std::move(output_dir)),
                                                                    logger_(logger)
        {
        }

        void generate_lua_bindings() const;

    private:
        static constexpr auto DUMMY_CPP_FILENAME = "dummy.cpp";
        static constexpr auto GENERATED_BINDINGS_FILENAME = "generated_bindings.cpp";

        fs::path include_dir_;
        fs::path wrapper_dir_;
        fs::path config_yaml_;
        fs::path output_dir_;
        std::ostream* logger_;

        void log(std::string_view message) const
        {
            if (logger_)
            {
                *logger_ << message << std::endl;
            }
        }
    };
} // namespace codegen
