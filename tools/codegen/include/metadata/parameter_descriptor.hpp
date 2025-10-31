/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <string>
#include <utility>

namespace codegen::metadata
{
    class ParameterDescriptor final
    {
    public:
        ParameterDescriptor() = default;

        explicit ParameterDescriptor(std::string name, std::string type_name): name_(std::move(name)),
                                                                               type_name_(std::move(type_name))
        {
        }

        const std::string& name() const noexcept
        {
            return name_;
        }

        const std::string& type_name() const noexcept
        {
            return type_name_;
        }

        ParameterDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        ParameterDescriptor& set_type_name(std::string type_name)
        {
            type_name_ = std::move(type_name);
            return *this;
        }

    private:
        std::string name_;
        std::string type_name_;
    };
} // namespace codegen::metadata
