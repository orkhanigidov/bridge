#pragma once

namespace engine::meta
{
    class ParameterDescriptor final
    {
    public:
        ParameterDescriptor() = default;

        [[nodiscard]] const std::string& name() const noexcept
        {
            return m_name;
        }

        [[nodiscard]] const std::string& type() const noexcept
        {
            return m_type;
        }

        void set_name(const std::string& name) noexcept
        {
            m_name = name;
        }

        void set_type(const std::string& type) noexcept
        {
            m_type = type;
        }

    private:
        std::string m_name;
        std::string m_type;
    };
} // namespace engine::meta
