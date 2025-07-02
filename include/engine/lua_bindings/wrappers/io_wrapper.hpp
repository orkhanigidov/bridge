#pragma once

#include "pch.hpp"

namespace engine::registration::wrappers
{
    class io_wrapper
    {
      public:
        std::string_view read(std::string_view path) const;
        bool write(std::string_view path, std::string_view content) const;
    };
} // namespace engine::registration::wrappers