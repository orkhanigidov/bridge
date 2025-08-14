#include "bindings/lua/registrar.hpp"

namespace bindings::lua {
    Registrar::Registrar()
    {
        m_lua.open_libraries(sol::lib::base);
    }

    void Registrar::register_bindings()
    {
        register_classes(m_lua);
        register_free_functions(m_lua);
    }

    std::vector<const reflection::FunctionDescriptor*>
    Registrar::registered_functions() const
    {
        size_t total_functions = m_free_functions.size();
        for (const auto& class_desc: m_classes | std::views::values) {
            total_functions += class_desc->member_functions().size();
        }

        std::vector<const reflection::FunctionDescriptor*> functions;
        functions.reserve(total_functions);
        for (const auto& function_desc: m_free_functions | std::views::values) {
            functions.emplace_back(function_desc.get());
        }

        for (const auto& class_desc: m_classes | std::views::values) {
            for (const auto& method_desc: class_desc->member_functions()) {
                functions.emplace_back(&method_desc);
            }
        }
        return functions;
    }

    bool Registrar::has_class(const std::string& name) const noexcept
    {
        return m_classes.contains(name);
    }

    const reflection::ClassDescriptor*
    Registrar::get_class(const std::string& name) const noexcept
    {
        if (has_class(name)) { return m_classes.find(name)->second.get(); }
        return nullptr;
    }

    bool Registrar::has_free_function(const std::string& name) const noexcept
    {
        return m_free_functions.contains(name);
    }

    const reflection::FunctionDescriptor*
    Registrar::get_free_function(const std::string& name) const noexcept
    {
        if (has_free_function(name)) {
            return m_free_functions.find(name)->second.get();
        }
        return nullptr;
    }
} // namespace bindings::lua
