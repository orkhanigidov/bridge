#pragma once

#include "parameter_descriptor.hpp"

namespace reflection {
    class ConstructorDescriptor final {
    public:
        ConstructorDescriptor() = default;
        explicit ConstructorDescriptor(std::string name)
            : m_name(std::move(name))
        {}

        std::string_view name() const noexcept { return m_name; }

        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return m_parameters;
        }

        std::string_view signature() const noexcept { return m_signature; }

        ConstructorDescriptor& set_name(std::string name)
        {
            m_name = std::move(name);
            return *this;
        }

        ConstructorDescriptor& set_signature(std::string signature)
        {
            m_signature = std::move(signature);
            return *this;
        }

        ConstructorDescriptor&
        add_parameter(ParameterDescriptor parameter_descriptor)
        {
            m_parameters.emplace_back(std::move(parameter_descriptor));
            return *this;
        }

    private:
        std::string m_name;
        std::vector<ParameterDescriptor> m_parameters;
        std::string m_signature;
    };
} // namespace reflection
