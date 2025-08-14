#pragma once

namespace reflection {
    class VariableDescriptor {
    public:
        VariableDescriptor() = default;
        explicit VariableDescriptor(std::string name): m_name(std::move(name))
        {}

        std::string_view name() const noexcept { return m_name; }

        std::string_view type() const noexcept { return m_type; }

        bool is_static() const noexcept { return m_is_static; }

        bool is_const() const noexcept { return m_is_const; }

        VariableDescriptor& set_name(std::string name)
        {
            m_name = std::move(name);
            return *this;
        }

        VariableDescriptor& set_type(std::string type)
        {
            m_type = std::move(type);
            return *this;
        }

        VariableDescriptor& set_static(bool static_state) noexcept
        {
            m_is_static = static_state;
            return *this;
        }

        VariableDescriptor& set_const(bool const_state) noexcept
        {
            m_is_const = const_state;
            return *this;
        }

    private:
        std::string m_name;
        std::string m_type;
        bool m_is_static{false};
        bool m_is_const{false};
    };
} // namespace reflection
