#pragma once

#include "reflection/class_descriptor.hpp"
#include "reflection/function_descriptor.hpp"
#include <sol/sol.hpp>

namespace bindings::lua {
    class Registrar final {
    public:
        static Registrar& instance()
        {
            static Registrar registrar;
            return registrar;
        }

        void register_bindings();

        static void register_classes(sol::state& lua);
        static void register_free_functions(sol::state& lua);

        std::vector<const reflection::FunctionDescriptor*>
        registered_functions() const;
        bool has_class(const std::string& name) const noexcept;
        const reflection::ClassDescriptor*
        get_class(const std::string& name) const noexcept;
        bool has_free_function(const std::string& name) const noexcept;
        const reflection::FunctionDescriptor*
        get_free_function(const std::string& name) const noexcept;

        sol::state& lua() { return m_lua; }

    private:
        Registrar();

        sol::state m_lua;
        std::unordered_map<std::string,
                           std::unique_ptr<reflection::ClassDescriptor>>
                m_classes;
        std::unordered_map<std::string,
                           std::unique_ptr<reflection::FunctionDescriptor>>
                m_free_functions;
    };
} // namespace bindings::lua
