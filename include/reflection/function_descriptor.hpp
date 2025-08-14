#pragma once

#include "parameter_descriptor.hpp"

namespace reflection {
    class FunctionDescriptor {
    public:
        FunctionDescriptor() = default;
        explicit FunctionDescriptor(std::string name): m_name(std::move(name))
        {}

        std::string_view name() const noexcept { return m_name; }

        std::string_view return_type() const noexcept { return m_return_type; }

        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return m_parameters;
        }

        bool is_static() const noexcept { return m_is_const; }

        bool is_const() const noexcept { return m_is_const; }

        std::string_view signature() const noexcept { return m_signature; }

        FunctionDescriptor& set_name(std::string name)
        {
            m_name = std::move(name);
            return *this;
        }

        FunctionDescriptor& set_return_type(std::string type)
        {
            m_return_type = std::move(type);
            return *this;
        }

        FunctionDescriptor& set_static(bool static_state) noexcept
        {
            m_is_static = static_state;
            return *this;
        }

        FunctionDescriptor& set_const(bool const_state) noexcept
        {
            m_is_const = const_state;
            return *this;
        }

        FunctionDescriptor& set_signature(std::string signature)
        {
            m_signature = std::move(signature);
            return *this;
        }

        FunctionDescriptor&
        add_parameter(ParameterDescriptor parameter_descriptor)
        {
            m_parameters.emplace_back(std::move(parameter_descriptor));
            return *this;
        }

    private:
        std::string m_name;
        std::string m_return_type;
        std::vector<ParameterDescriptor> m_parameters;
        bool m_is_static{false};
        bool m_is_const{false};
        std::string m_signature;
    };
} // namespace reflection
