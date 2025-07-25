#pragma once

#include "parameter_descriptor.hpp"

namespace engine::meta
{
    class FunctionDescriptor final
    {
    public:
        FunctionDescriptor() = default;

        [[nodiscard]] const std::string& name() const noexcept
        {
            return m_name;
        }

        [[nodiscard]] const std::string& return_type() const noexcept
        {
            return m_return_type;
        }

        [[nodiscard]] const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return m_parameters;
        }

        [[nodiscard]] bool is_static() const noexcept
        {
            return m_is_const;
        }

        [[nodiscard]] bool is_const() const noexcept
        {
            return m_is_const;
        }

        [[nodiscard]] const std::string& signature() const noexcept
        {
            return m_signature;
        }

        void set_name(const std::string& name) noexcept
        {
            m_name = name;
        }

        void set_return_type(const std::string& return_type) noexcept
        {
            m_return_type = return_type;
        }

        void add_parameter(const ParameterDescriptor& parameter) noexcept
        {
            m_parameters.emplace_back(parameter);
        }

        void set_static(bool is_static) noexcept
        {
            m_is_static = is_static;
        }

        void set_const(bool is_const) noexcept
        {
            m_is_const = is_const;
        }

        void set_signature(const std::string& signature) noexcept
        {
            m_signature = signature;
        }

    private:
        std::string m_name;
        std::string m_return_type;
        std::vector<ParameterDescriptor> m_parameters;
        bool m_is_static{false};
        bool m_is_const{false};
        std::string m_signature;
    };
} // namespace engine::meta
