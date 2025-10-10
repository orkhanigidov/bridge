#include "bindings/lua/lua_binder.hpp"

namespace engine::bindings::lua {

    void LuaBinder::register_bindings(sol::state& lua)
    {
        try {
            const auto start_time = std::chrono::high_resolution_clock::now();

            register_members(lua);
            register_non_members(lua);
            // register_manual_bindings(lua);

            const auto end_time = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

            std::cout << "Lua bindings successfully registered in " << duration.count() << " milliseconds" << std::endl;
        } catch (const sol::error& e) {
            std::cerr << "Error registering Lua bindings: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Unhandled exception while registering Lua bindings: " << e.what() << std::endl;
        }

        const size_t total_classes = classes_.size();
        const size_t total_free_functions = free_functions_.size();
        
        if (total_classes == 0 && total_free_functions == 0) {
            std::cout << "No classes or free functions were registered" << std::endl;
        } else {
            std::cout << std::format("Registered {} class{} and {} free function{}",
                total_classes, total_classes == 1 ? "" : "es",
                total_free_functions, total_free_functions == 1 ? "" : "s") << std::endl;
        }
    }

    std::vector<const metadata::FunctionDescriptor*> LuaBinder::registered_functions() const
    {
        size_t total_functions = free_functions_.size();
        for (const auto& [_, class_desc]: classes_) {
            total_functions += class_desc->member_functions().size();
        }

        std::vector<const metadata::FunctionDescriptor*> functions;
        functions.reserve(total_functions);
        
        for (const auto& [_, function_desc]: free_functions_) {
            functions.emplace_back(function_desc.get());
        }

        for (const auto& [_, class_desc]: classes_) {
            for (const auto& function_desc: class_desc->member_functions()) {
                functions.emplace_back(&function_desc);
            }
        }
        return functions;
    }

    bool LuaBinder::contains_class(const std::string& class_name) const noexcept
    {
        return classes_.contains(class_name);
    }

    const metadata::ClassDescriptor* LuaBinder::get_class(const std::string& class_name) const noexcept
    {
        if (const auto it = classes_.find(class_name); it != classes_.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    bool LuaBinder::contains_free_function(const std::string& function_name) const noexcept
    {
        return free_functions_.contains(function_name);
    }

    const metadata::FunctionDescriptor* LuaBinder::get_free_function(const std::string& function_name) const noexcept
    {
        if (const auto it = free_functions_.find(function_name); it != free_functions_.end()) {
            return it->second.get();
        }
        return nullptr;
    }

} // namespace engine::bindings::lua
