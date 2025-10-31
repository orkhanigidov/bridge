/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <filesystem>
#include <ostream>
#include <stdexcept>
#include <string_view>
#include <utility>

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
        explicit Engine(std::filesystem::path include_dir, std::filesystem::path wrapper_dir, std::filesystem::path config_yaml,
                        std::filesystem::path output_dir, std::ostream* logger): include_dir_(std::move(include_dir)),
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

        std::filesystem::path include_dir_;
        std::filesystem::path wrapper_dir_;
        std::filesystem::path config_yaml_;
        std::filesystem::path output_dir_;
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
