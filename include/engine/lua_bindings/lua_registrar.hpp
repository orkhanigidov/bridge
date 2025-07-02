#pragma once

#include "engine/metadata/class_descriptor.hpp"
#include "engine/metadata/function_descriptor.hpp"
#include "pch.hpp"

namespace engine::lua_bindings
{
    class lua_registrar final
    {
      public:
        static lua_registrar& instance()
        {
            static lua_registrar registrar;
            return registrar;
        }

        sol::state& get_lua()
        {
            return lua_;
        }

        void register_all();

        std::vector<const metadata::function_descriptor*> get_registered_functions() const;

        bool has_class(std::string_view name) const;
        const metadata::class_descriptor* get_class(std::string_view name) const;

        bool has_function(std::string_view name) const;
        const metadata::function_descriptor* get_function(std::string_view name) const;

      private:
        lua_registrar();

        sol::state lua_;

        std::unordered_map<std::string, std::unique_ptr<metadata::class_descriptor>> classes_;
        std::unordered_map<std::string, std::unique_ptr<metadata::function_descriptor>> functions_;
    };

    void register_classes(sol::state& lua);
    void register_global_functions(sol::state& lua);
} // namespace engine::lua_bindings