#pragma once

namespace codegen::metadata
{
    struct EnumDescriptor
    {
        std::string name;
        long long value;
    };

    class EnumeratorDescriptor final
    {
    public:
        EnumeratorDescriptor() = default;

        explicit EnumeratorDescriptor(std::string name) : name_(std::move(name))
        {
        }

        const std::string& name() const noexcept
        {
            return name_;
        }

        const std::vector<EnumDescriptor>& enums() const noexcept
        {
            return enums_;
        }

        EnumeratorDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        EnumeratorDescriptor& add_enum(EnumDescriptor enum_)
        {
            enums_.emplace_back(std::move(enum_));
            return *this;
        }

    private:
        std::string name_;
        std::vector<EnumDescriptor> enums_;
    };
} // codegen::metadata
