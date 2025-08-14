#pragma once

#include "constructor_descriptor.hpp"
#include "data_member_descriptor.hpp"
#include "member_function_descriptor.hpp"

namespace reflection {
    class ClassDescriptor final {
    public:
        ClassDescriptor() = default;
        explicit ClassDescriptor(std::string name): m_name(std::move(name)) {}

        std::string_view name() const noexcept { return m_name; }

        const std::vector<std::string>& base_class_names() const noexcept
        {
            return m_base_class_names;
        }

        const std::vector<ConstructorDescriptor>& constructors() const noexcept
        {
            return m_constructors;
        }

        const std::vector<DataMemberDescriptor>& data_members() const noexcept
        {
            return m_data_members;
        }

        const std::vector<MemberFunctionDescriptor>&
        member_functions() const noexcept
        {
            return m_member_functions;
        }

        ClassDescriptor& set_name(std::string name)
        {
            m_name = std::move(name);
            return *this;
        }

        ClassDescriptor& add_base_class_name(std::string base_class_name)
        {
            m_base_class_names.emplace_back(std::move(base_class_name));
            return *this;
        }

        ClassDescriptor&
        add_constructor(ConstructorDescriptor constructor_descriptor)
        {
            m_constructors.emplace_back(std::move(constructor_descriptor));
            return *this;
        }

        ClassDescriptor&
        add_data_member(DataMemberDescriptor data_member_descriptor)
        {
            m_data_members.emplace_back(std::move(data_member_descriptor));
            return *this;
        }

        ClassDescriptor&
        add_member_function(MemberFunctionDescriptor member_function_descriptor)
        {
            m_member_functions.emplace_back(
                    std::move(member_function_descriptor));
            return *this;
        }

    private:
        std::string m_name;
        std::vector<std::string> m_base_class_names;
        std::vector<ConstructorDescriptor> m_constructors;
        std::vector<DataMemberDescriptor> m_data_members;
        std::vector<MemberFunctionDescriptor> m_member_functions;
    };
} // namespace reflection
