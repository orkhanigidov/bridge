#pragma once

#include "parameter_descriptor.hpp"
#include "scope.hpp"

namespace metadata {

    class FunctionDescriptor final {
    public:
        FunctionDescriptor() = default;
        
        explicit FunctionDescriptor(Scope scope, std::string name):
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

        const std::string& return_type_name() const noexcept
        {
            return m_return_type_name;
        }

        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return m_parameters;
        }

        bool is_static() const noexcept
        {
            return m_is_static;
        }

        bool is_const() const noexcept
        {
            return m_is_const;
        }

        const std::string& signature() const noexcept
        {
            return m_signature;
        }

        FunctionDescriptor& set_scope(Scope scope) noexcept
        {
            m_scope = scope;
            return *this;
        }

        FunctionDescriptor& set_name(std::string name)
        {
            m_name = std::move(name);
            return *this;
        }

        FunctionDescriptor& set_return_type_name(std::string type)
        {
            m_return_type_name = std::move(type);
            return *this;
        }

        FunctionDescriptor& add_parameter(ParameterDescriptor parameter)
        {
            m_parameters.emplace_back(std::move(parameter));
            return *this;
        }

        FunctionDescriptor& set_static(bool value) noexcept
        {
            m_is_static = value;
            return *this;
        }

        FunctionDescriptor& set_const(bool value) noexcept
        {
            m_is_const = value;
            return *this;
        }

        FunctionDescriptor& set_signature(std::string signature)
        {
            m_signature = std::move(signature);
            return *this;
        }

    private:
        Scope m_scope{Scope::Global};
        std::string m_name;
        std::string m_return_type_name;
        std::vector<ParameterDescriptor> m_parameters;
        bool m_is_static{false};
        bool m_is_const{false};
        std::string m_signature;
    };

} // namespace metadata
