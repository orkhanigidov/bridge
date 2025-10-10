#pragma once

#include "parameter_descriptor.hpp"

namespace engine::metadata {

    class ConstructorDescriptor final {
    public:
        ConstructorDescriptor() = default;
        
        explicit ConstructorDescriptor(std::string class_name):
            class_name_(std::move(class_name)) {}

        const std::string& class_name() const noexcept
        {
            return class_name_;
        }

        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return parameters_;
        }

        const std::string& signature() const noexcept
        {
            return signature_;
        }

        ConstructorDescriptor& set_class_name(std::string class_name)
        {
            class_name_ = std::move(class_name);
            return *this;
        }

        ConstructorDescriptor& add_parameter(ParameterDescriptor parameter)
        {
            parameters_.emplace_back(std::move(parameter));
            return *this;
        }

        ConstructorDescriptor& set_signature(std::string signature)
        {
            signature_ = std::move(signature);
            return *this;
        }

    private:
        std::string class_name_;
        std::vector<ParameterDescriptor> parameters_;
        std::string signature_;
    };

} // namespace engine::metadata
