#include "bindings/lua/registry.hpp"

namespace engine::bindings::lua {

    Registry::Registry()
    {
        m_lua.open_libraries(sol::lib::base);
    }

    void Registry::register_bindings()
    {
        try {
            const auto start_time = std::chrono::high_resolution_clock::now();

            register_members(m_lua);
            register_non_members(m_lua);
            register_manual_bindings(m_lua);

            const auto end_time = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

            std::cout << "Lua bindings successfully registered in " << duration.count() << " milliseconds" << std::endl;
        } catch (const sol::error& e) {
            std::cerr << "Error registering Lua bindings: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Unhandled exception while registering Lua bindings: " << e.what() << std::endl;
        }

        const size_t total_classes = m_classes.size();
        const size_t total_free_functions = m_free_functions.size();
        
        if (total_classes == 0 && total_free_functions == 0) {
            std::cout << "No classes or free functions were registered" << std::endl;
        } else {
            std::cout << std::format("Registered {} class{} and {} free function{}",
                total_classes, total_classes == 1 ? "" : "es",
                total_free_functions, total_free_functions == 1 ? "" : "s") << std::endl;
        }
    }

    std::vector<const metadata::FunctionDescriptor*> Registry::registered_functions() const
    {
        size_t total_functions = m_free_functions.size();
        for (const auto& [_, class_desc]: m_classes) {
            total_functions += class_desc->member_functions().size();
        }

        std::vector<const metadata::FunctionDescriptor*> functions;
        functions.reserve(total_functions);
        
        for (const auto& [_, function_desc]: m_free_functions) {
            functions.emplace_back(function_desc.get());
        }

        for (const auto& [_, class_desc]: m_classes) {
            for (const auto& function_desc: class_desc->member_functions()) {
                functions.emplace_back(&function_desc);
            }
        }
        return functions;
    }

    bool Registry::contains_class(const std::string& class_name) const noexcept
    {
        return m_classes.contains(class_name);
    }

    const metadata::ClassDescriptor* Registry::get_class(const std::string& class_name) const noexcept
    {
        if (const auto it = m_classes.find(class_name); it != m_classes.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    bool Registry::contains_free_function(const std::string& function_name) const noexcept
    {
        return m_free_functions.contains(function_name);
    }

    const metadata::FunctionDescriptor* Registry::get_free_function(const std::string& function_name) const noexcept
    {
        if (const auto it = m_free_functions.find(function_name); it != m_free_functions.end()) {
            return it->second.get();
        }
        return nullptr;
    }

} // namespace engine::bindings::lua
