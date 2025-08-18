#pragma once

#include "scope.hpp"

namespace metadata {

    class VariableDescriptor final {
    public:
        VariableDescriptor() = default;
        
        explicit VariableDescriptor(Scope scope, std::string name):
            m_scope(scope),
            m_name(std::move(name)) {}

        Scope scope() const noexcept
        {
            return m_scope;
        }

        const std::string& name() const noexcept
        {
            return m_name;
        }

        const std::string& type_name() const noexcept
        {
            return m_type_name;
        }

        bool is_static() const noexcept
        {
            return m_is_static;
        }

        bool is_const() const noexcept
        {
            return m_is_const;
        }

        VariableDescriptor& set_scope(Scope scope) noexcept
        {
            m_scope = scope;
            return *this;
        }

        VariableDescriptor& set_name(std::string name)
        {
            m_name = std::move(name);
            return *this;
        }

        VariableDescriptor& set_type_name(std::string type)
        {
            m_type_name = std::move(type);
            return *this;
        }

        VariableDescriptor& set_static(bool value) noexcept
        {
            m_is_static = value;
            return *this;
        }

        VariableDescriptor& set_const(bool value) noexcept
        {
            m_is_const = value;
            return *this;
        }

    private:
        Scope m_scope{Scope::Global};
        std::string m_name;
        std::string m_type_name;
        bool m_is_static{false};
        bool m_is_const{false};
    };

} // namespace metadata
