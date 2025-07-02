#include "engine/lua_bindings/lua_registrar.hpp"

#include "engine/metadata/class_descriptor.hpp"
#include "engine/metadata/function_descriptor.hpp"
#include "pch.hpp"

namespace engine::lua_bindings
{
    lua_registrar::lua_registrar()
    {
        lua_.open_libraries(sol::lib::base);
    }

    void lua_registrar::register_all()
    {
        register_classes(lua_);
        register_global_functions(lua_);
    }

    std::vector<const metadata::function_descriptor*>
    lua_registrar::get_registered_functions() const
    {
        std::vector<const metadata::function_descriptor*> functions;
        functions.reserve(functions_.size());

        for (const auto& [key, function] : functions_)
        {
            functions.emplace_back(function.get());
        }

        return functions;
    }

    bool lua_registrar::has_class(std::string_view name) const
    {
        return classes_.find(std::string(name)) != classes_.end();
    }

    const metadata::class_descriptor* lua_registrar::get_class(std::string_view name) const
    {
        if (has_class(name))
        {
            return classes_.find(std::string(name))->second.get();
        }

        return nullptr;
    }

    bool lua_registrar::has_function(std::string_view name) const
    {
        return functions_.find(std::string(name)) != functions_.end();
    }

    const metadata::function_descriptor* lua_registrar::get_function(std::string_view name) const
    {
        if (has_function(name))
        {
            return functions_.find(std::string(name))->second.get();
        }

        return nullptr;
    }
} // namespace engine::lua_bindings