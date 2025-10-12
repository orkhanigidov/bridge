#pragma once

namespace codegen::metadata
{
    struct EnumeratorDescriptor
    {
        std::string name;
        long long value;
    };

    class EnumDescriptor final
    {
    public:
        EnumDescriptor() = default;

        explicit EnumDescriptor(std::string name) : name_(std::move(name))
        {
        }

        const std::string& name() const noexcept
        {
            return name_;
        }

        const std::vector<EnumeratorDescriptor>& enumerators() const noexcept
        {
            return enumerators_;
        }

        EnumDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        EnumDescriptor& add_enumerator(EnumeratorDescriptor enumerator)
        {
            enumerators_.emplace_back(std::move(enumerator));
            return *this;
        }

    private:
        std::string name_;
        std::vector<EnumeratorDescriptor> enumerators_;
    };
} // codegen::metadata
