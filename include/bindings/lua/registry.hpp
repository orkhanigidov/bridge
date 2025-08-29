#pragma once

#include "metadata/class_descriptor.hpp"
#include "metadata/function_descriptor.hpp"

#include <sol/sol.hpp>

namespace bindings::lua {

    class Registry final {
    public:
        static Registry& instance()
        {
            static Registry registry;
            return registry;
        }

        void register_bindings();

        std::vector<const metadata::FunctionDescriptor*> registered_functions() const;
        
        bool contains_class(const std::string& class_name) const noexcept;
        const metadata::ClassDescriptor* get_class(const std::string& class_name) const noexcept;
        
        bool contains_free_function(const std::string& function_name) const noexcept;
        const metadata::FunctionDescriptor* get_free_function(const std::string& function_name) const noexcept;

        sol::state& lua()
        {
            return m_lua;
        }

        const sol::state& lua() const
        {
            return m_lua;
        }

    private:
        Registry();

        sol::state m_lua;
        std::unordered_map<std::string, std::unique_ptr<metadata::ClassDescriptor>> m_classes;
        std::unordered_map<std::string, std::unique_ptr<metadata::FunctionDescriptor>> m_free_functions;
    };

    void register_members(sol::state& lua);
    void register_non_members(sol::state& lua);
    void register_manual_bindings(sol::state& lua);

} // namespace bindings::lua
