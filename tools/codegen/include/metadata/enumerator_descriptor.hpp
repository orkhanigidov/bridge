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

        const std::vector<EnumDescriptor>& enumerators() const noexcept
        {
            return enumerators_;
        }

        EnumeratorDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        EnumeratorDescriptor& add_enumerator(EnumDescriptor enumerator)
        {
            enumerators_.emplace_back(std::move(enumerator));
            return *this;
        }

    private:
        std::string name_;
        std::vector<EnumDescriptor> enumerators_;
    };
} // codegen::metadata
