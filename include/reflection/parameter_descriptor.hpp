#pragma once

namespace reflection {
    class ParameterDescriptor final {
    public:
        ParameterDescriptor() = default;
        explicit ParameterDescriptor(std::string name): m_name(std::move(name))
        {}

        std::string_view name() const noexcept { return m_name; }

        std::string_view type() const noexcept { return m_type; }

        ParameterDescriptor& set_name(std::string name)
        {
            m_name = std::move(name);
            return *this;
        }

        ParameterDescriptor& set_type(std::string type)
        {
            m_type = std::move(type);
            return *this;
        }

    private:
        std::string m_name;
        std::string m_type;
    };
} // namespace reflection
