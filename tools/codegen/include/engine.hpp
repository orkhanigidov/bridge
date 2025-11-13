/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file engine.hpp
 * @brief Declares the Engine class for generating Lua bindings.
 */

#pragma once

#include <filesystem>
#include <ostream>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace codegen
{
    /**
     * @class EngineException
     * @brief Exception thrown by Engine on errors.
     */
    class EngineException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    /**
     * @class Engine
     * @brief Main engine for generating Lua bindings from C++ headers and configuration.
     *
     * The Engine processes include directories, wrapper directories, and a YAML configuration
     * to generate Lua bindings, writing output to the specified directory. Logging is supported
     * via an optional std::ostream pointer.
     */
    class Engine final
    {
    public:
        /**
         * @brief Constructs the Engine.
         * @param include_dir Directory containing C++ headers to process.
         * @param wrapper_dir Directory for wrapper files.
         * @param manual_register_dir Directory for manual binding header files.
         * @param config_yaml Path to the YAML configuration file.
         * @param output_dir Directory to write generated files to.
         * @param logger Optional logger for status and error messages.
         */
        explicit Engine(std::filesystem::path include_dir, std::filesystem::path wrapper_dir,
                        std::filesystem::path manual_register_dir, std::filesystem::path config_yaml,
                        std::filesystem::path output_dir, std::ostream* logger) : include_dir_(std::move(include_dir)),
                                                                                  wrapper_dir_(std::move(wrapper_dir)),
                                                                                  manual_register_dir_(std::move(manual_register_dir)),
                                                                                  config_yaml_(std::move(config_yaml)),
                                                                                  output_dir_(std::move(output_dir)),
                                                                                  logger_(logger)
        {
        }

        /**
         * @brief Generates Lua bindings based on the provided configuration and directories.
         */
        void generate_lua_bindings() const;

    private:
        /**
         * @brief Name of the dummy C++ file.
         */
        static constexpr auto DUMMY_CPP_FILENAME = "dummy.cpp";

        /**
         * @brief Name of the generated bindings file.
         */
        static constexpr auto GENERATED_BINDINGS_FILENAME = "generated_bindings.cpp";

        /**
         * @brief Directory with C++ headers.
         */
        std::filesystem::path include_dir_;

        /**
         * @brief Directory for wrapper files.
         */
        std::filesystem::path wrapper_dir_;

        /**
         * @brief Directory for manual binding files.
         */
        std::filesystem::path manual_register_dir_;

        /**
         * @brief Path to YAML configuration.
         */
        std::filesystem::path config_yaml_;

        /**
         * @brief Output directory for generated files.
         */
        std::filesystem::path output_dir_;

        /**
         * @brief Optional logger.
         */
        std::ostream* logger_;

        /**
         * @brief Logs a message if a logger is provided.
         * @param message The message to log.
         */
        void log(std::string_view message) const
        {
            if (logger_)
            {
                *logger_ << message << std::endl;
            }
        }
    };
} // namespace codegen
