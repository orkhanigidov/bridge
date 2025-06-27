#pragma once

#include "pch.hpp"

namespace engine::registration
{
    class FunctionRegistrar
    {
      public:
        FunctionRegistrar(sol::state& lua) : lua_(lua) {}

        template <typename Fn>
        FunctionRegistrar& func(std::string_view name, Fn&& fn)
        {
            lua_.set_function(name, std::forward<Fn>(fn));
            return *this;
        }

        void register_function() {}

      private:
        sol::state& lua_;
    };
} // namespace engine::registration