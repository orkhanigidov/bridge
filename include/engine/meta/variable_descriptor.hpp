#pragma once

namespace engine::meta
{
    class VariableDescriptor final
    {
    public:
        VariableDescriptor() = default;

        const std::string& name() const noexcept
        {
            return m_name;
        }

        const std::string& type() const noexcept
        {
            return m_type;
        }

        bool is_static() const noexcept
        {
            return m_is_static;
        }

        bool is_const() const noexcept
        {
            return m_is_const;
        }

        void set_name(const std::string& name) noexcept
        {
            m_name = name;
        }

        void set_type(const std::string& type) noexcept
        {
            m_type = type;
        }

        void set_static(bool is_static) noexcept
        {
            m_is_static = is_static;
        }

        void set_const(bool is_const) noexcept
        {
            m_is_const = is_const;
        }

    private:
        std::string m_name;
        std::string m_type;
        bool m_is_static{false};
        bool m_is_const{false};
    };
} // namespace engine::meta
