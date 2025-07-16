#pragma once

#include "../../registration/type_descriptor.hpp"
#include "parameter_descriptor.hpp"

namespace engine::meta
{
    class FunctionDescriptor final
    {
      public:
        FunctionDescriptor() = default;

        const std::string& name() const noexcept
        {
            return name_;
        }

        void setName(const std::string& name)
        {
            name_ = name;
        }

        const std::string& returnType() const noexcept
        {
            return return_type_;
        }

        void setReturnType(const std::string& return_type)
        {
            return_type_ = return_type;
        }

        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return parameters_;
        }

        void addParameter(const ParameterDescriptor& parameter)
        {
            parameters_.emplace_back(parameter);
        }

        bool isStatic() const noexcept
        {
            return is_static_;
        }

        void setStatic(bool is_static)
        {
            is_static_ = is_static;
        }

        const std::string& signature() const noexcept
        {
            return signature_;
        }

        void setSignature(const std::string& signature)
        {
            signature_ = signature;
        }

      private:
        std::string name_;
        std::string return_type_;
        std::vector<ParameterDescriptor> parameters_;
        bool is_static_{false};
        std::string signature_;
    };
} // namespace engine::meta