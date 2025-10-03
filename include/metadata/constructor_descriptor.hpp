#pragma once

#include "parameter_descriptor.hpp"

namespace engine::metadata {

    class ConstructorDescriptor final {
    public:
        ConstructorDescriptor() = default;
        
        explicit ConstructorDescriptor(std::string class_name):
            m_class_name(std::move(class_name)) {}

        const std::string& class_name() const noexcept
        {
            return m_class_name;
        }

        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return m_parameters;
        }

        const std::string& signature() const noexcept
        {
            return m_signature;
        }

        ConstructorDescriptor& set_class_name(std::string class_name)
        {
            m_class_name = std::move(class_name);
            return *this;
        }

        ConstructorDescriptor& add_parameter(ParameterDescriptor parameter)
        {
            m_parameters.emplace_back(std::move(parameter));
            return *this;
        }

        ConstructorDescriptor& set_signature(std::string signature)
        {
            m_signature = std::move(signature);
            return *this;
        }

    private:
        std::string m_class_name;
        std::vector<ParameterDescriptor> m_parameters;
        std::string m_signature;
    };

} // namespace engine::metadata
