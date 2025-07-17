#pragma once

#include "parameter_descriptor.hpp"

namespace engine::meta
{
    class ConstructorDescriptor final
    {
    public:
        ConstructorDescriptor() = default;

        const std::string& name() const noexcept
        {
            return m_name;
        }

        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return m_parameters;
        }

        const std::string& signature() const noexcept
        {
            return m_signature;
        }

        void set_name(const std::string& name) noexcept
        {
            m_name = name;
        }

        void add_parameter(const ParameterDescriptor& parameter) noexcept
        {
            m_parameters.emplace_back(parameter);
        }

        void set_signature(const std::string& signature) noexcept
        {
            m_signature = signature;
        }

    private:
        std::string m_name;
        std::vector<ParameterDescriptor> m_parameters;
        std::string m_signature;
    };
} // namespace engine::meta
