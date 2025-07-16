#pragma once

namespace engine::meta
{
    class ParameterDescriptor final
    {
      public:
        ParameterDescriptor() = default;

        const std::string& name() const noexcept
        {
            return name_;
        }

        void setName(const std::string& name)
        {
            name_ = name;
        }

        const std::string& type() const noexcept
        {
            return type_;
        }

        void setType(const std::string& type)
        {
            type_ = type;
        }

      private:
        std::string name_;
        std::string type_;
    };
} // namespace engine::meta