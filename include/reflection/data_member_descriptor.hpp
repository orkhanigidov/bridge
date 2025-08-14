#pragma once

#include "variable_descriptor.hpp"

namespace reflection {
    class DataMemberDescriptor : public VariableDescriptor {
        using VariableDescriptor::VariableDescriptor;
    };
} // namespace reflection
