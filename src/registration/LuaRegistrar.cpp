#include "pch.hpp"
#include "registration/LuaRegistrar.hpp"

using namespace engine::model;

namespace engine::registration
{
    void LuaRegistrar::load_generated(GeneratedRegistry&& registry)
    {
        classes_   = std::move(registry.classes);
        functions_ = std::move(registry.functions);

        std::cout << "Loaded " << classes_.size() << " classes, " << functions_.size()
                  << " functions" << std::endl;
    }

    void LuaRegistrar::register_all()
    {
        register_classes(lua_);
        register_global_functions(lua_);
    }

    std::vector<const FuncDesc*> LuaRegistrar::get_registered_functions() const
    {
        std::vector<const FuncDesc*> functions;
        functions.reserve(functions_.size());

        for (const auto& [key, function] : functions_)
        {
            functions.emplace_back(function.get());
        }

        return functions;
    }

    bool LuaRegistrar::has_class(const std::string& name) const
    {
        return classes_.find(name) != classes_.end();
    }

    const ClassDesc* LuaRegistrar::get_class(const std::string& name) const
    {
        if (has_class(name))
        {
            return classes_.find(name)->second.get();
        }

        return nullptr;
    }

    bool LuaRegistrar::has_function(const std::string& name) const
    {
        return functions_.find(name) != functions_.end();
    }

    const FuncDesc* LuaRegistrar::get_function(const std::string& name) const
    {
        if (has_function(name))
        {
            return functions_.find(name)->second.get();
        }

        return nullptr;
    }
} // namespace engine::registration