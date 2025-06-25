#pragma once

#include "RegistryTypes.hpp"
#include "model/ClassDesc.hpp"
#include "model/FuncDesc.hpp"
#include "pch.hpp"
#include "sol/sol.hpp"

namespace engine::registration
{
    struct GeneratedRegistry
    {
        ClassMap classes;
        FuncMap functions;
    };

    class LuaRegistrar
    {
      public:
        static LuaRegistrar& instance()
        {
            static LuaRegistrar registrar;
            return registrar;
        }

        void register_all();

        void load_generated(GeneratedRegistry&& registry);

        std::vector<const model::FuncDesc*> get_registered_functions() const;

        bool has_class(const std::string& name) const;
        const model::ClassDesc* get_class(const std::string& name) const;

        bool has_function(const std::string& name) const;
        const model::FuncDesc* get_function(const std::string& name) const;

        sol::state& get_lua()
        {
            return lua_;
        }

      private:
        LuaRegistrar()
        {
            lua_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string,
                                sol::lib::table, sol::lib::debug);
        }

        sol::state lua_;

        ClassMap classes_{};
        FuncMap functions_{};
    };

    void register_classes(sol::state& lua);
    void register_global_functions(sol::state& lua);
} // namespace engine::registration