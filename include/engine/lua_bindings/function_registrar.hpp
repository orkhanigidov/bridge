#pragma once

#include "pch.hpp"

namespace engine::lua_bindings
{
    class function_registrar final
    {
      public:
        explicit function_registrar(sol::state& lua) : lua_(lua) {}

        template <typename Fn>
        function_registrar& add_function(std::string_view name, Fn&& fn)
        {
            lua_.set_function(name, std::forward<Fn>(fn));
            return *this;
        }

      private:
        sol::state& lua_;
    };
} // namespace engine::lua_bindings