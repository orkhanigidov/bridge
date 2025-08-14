#pragma once

#include "function_descriptor.hpp"

namespace reflection {
    class MemberFunctionDescriptor : public FunctionDescriptor {
        using FunctionDescriptor::FunctionDescriptor;
    };
} // namespace reflection
