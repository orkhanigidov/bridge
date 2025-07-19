#pragma once

namespace engine::bindings::lua
{
    class FreeFunctionRegistrar final
    {
    public:
        explicit FreeFunctionRegistrar(sol::state& lua) : m_lua(lua)
        {
        }

        template <typename Func>
        FreeFunctionRegistrar& function(const std::string& name, Func&& func)
        {
            m_lua.set_function(name, std::forward<Func>(func));
            return *this;
        }

    private:
        sol::state& m_lua;
    };
} // namespace engine::bindings::lua
