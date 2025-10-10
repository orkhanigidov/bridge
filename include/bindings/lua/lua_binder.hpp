#pragma once

#include "metadata/class_descriptor.hpp"
#include "metadata/function_descriptor.hpp"

#include <sol/sol.hpp>

namespace engine::bindings::lua {

    class LuaBinder final {
    public:
        LuaBinder() = default;

        void register_bindings(sol::state& lua);

        std::vector<const metadata::FunctionDescriptor*> registered_functions() const;
        
        bool contains_class(const std::string& class_name) const noexcept;
        const metadata::ClassDescriptor* get_class(const std::string& class_name) const noexcept;
        
        bool contains_free_function(const std::string& function_name) const noexcept;
        const metadata::FunctionDescriptor* get_free_function(const std::string& function_name) const noexcept;

    private:
        std::unordered_map<std::string, std::unique_ptr<metadata::ClassDescriptor>> classes_;
        std::unordered_map<std::string, std::unique_ptr<metadata::FunctionDescriptor>> free_functions_;
    };

    void register_members(sol::state& lua);
    void register_non_members(sol::state& lua);
    void register_manual_bindings(sol::state& lua);

} // namespace engine::bindings::lua
