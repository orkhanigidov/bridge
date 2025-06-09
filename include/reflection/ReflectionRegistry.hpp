#pragma once

#include "model/Class.hpp"
#include "model/Method.hpp"
#include "pch.hpp"

namespace engine::reflection
{
    class ReflectionRegistry
    {
      public:
        static ReflectionRegistry& instance()
        {
            static ReflectionRegistry registry;
            return registry;
        }

        void register_all_from_rttr();

        std::vector<const model::Method*> get_registered_methods() const;

        bool has_class(std::string_view name) const;
        const model::Class* get_class(std::string_view name) const;

        bool has_method(std::string_view name) const;
        const model::Method* get_method(std::string_view name) const;

      private:
        std::unordered_map<std::string, std::unique_ptr<model::Class>> classes_;
        std::unordered_map<std::string, std::unique_ptr<model::Method>> methods_;

        ReflectionRegistry() = default;

        void register_class(const rttr::type& type);
        void register_method(const rttr::method& method);
    };
} // namespace engine::reflection