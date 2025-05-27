#pragma once

#include "../pch.h"

#include "../model/Parameter.h"
#include "BaseNode.h"
#include "Result.h"

using namespace engine::model;

namespace engine::operation
{
    class InvokableNode : public BaseNode
    {
      public:
        explicit InvokableNode(std::string_view name, NodeType type);

        [[nodiscard]] const rttr::method& method() const noexcept
        {
            return method_;
        }

        [[nodiscard]] const std::unordered_map<std::string, Parameter>& parameters() const noexcept
        {
            return parameters_;
        }

        void set_parameter(std::string_view key, Parameter parameter);

        void               resolve() override;
        [[nodiscard]] bool is_valid() const noexcept override;

        virtual Result invoke() = 0;

      protected:
        rttr::method                               method_;
        std::unordered_map<std::string, Parameter> parameters_;
    };
} // namespace engine::operation