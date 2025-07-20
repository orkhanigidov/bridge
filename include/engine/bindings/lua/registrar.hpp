#pragma once

#include "engine/meta/class_descriptor.hpp"
#include "engine/meta/function_descriptor.hpp"

namespace engine::bindings::lua
{
    class Registrar final
    {
    public:
        static Registrar& instance()
        {
            static Registrar registrar;
            return registrar;
        }

        void register_bindings();
        static void register_classes(sol::state& lua);
        static void register_free_functions(sol::state& lua);

        std::vector<const meta::FunctionDescriptor*> registered_functions() const;
        bool has_class(const std::string& name) const noexcept;
        const meta::ClassDescriptor* get_class(const std::string& name) const noexcept;
        bool has_free_function(const std::string& name) const noexcept;
        const meta::FunctionDescriptor* get_free_function(const std::string& name) const noexcept;

        sol::state& lua()
        {
            return m_lua;
        }

    private:
        Registrar();

        sol::state m_lua;
        std::unordered_map<std::string, std::unique_ptr<meta::ClassDescriptor>> m_classes;
        std::unordered_map<std::string, std::unique_ptr<meta::FunctionDescriptor>> m_free_functions;
    };
} // namespace engine::bindings::lua
