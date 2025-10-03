#pragma once

namespace engine::metadata {

    class ParameterDescriptor final {
    public:
        ParameterDescriptor() = default;
        
        explicit ParameterDescriptor(std::string name):
            m_name(std::move(name)) {}

        const std::string& name() const noexcept
        {
            return m_name;
        }

        const std::string& type_name() const noexcept
        {
            return m_type_name;
        }

        ParameterDescriptor& set_name(std::string name)
        {
            m_name = std::move(name);
            return *this;
        }

        ParameterDescriptor& set_type_name(std::string type)
        {
            m_type_name = std::move(type);
            return *this;
        }

    private:
        std::string m_name;
        std::string m_type_name;
    };

} // namespace engine::metadata
