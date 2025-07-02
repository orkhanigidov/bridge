#pragma once

#include "pch.hpp"

namespace engine::bridge
{
    class native_bridge final
    {
      public:
        void process_execution_request(std::string_view data);
    };
} // namespace engine::bridge