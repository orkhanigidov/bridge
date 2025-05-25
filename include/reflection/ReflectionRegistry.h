#pragma once

#include "../model/Method.h"
#include "../model/Type.h"
#include "../pch.h"

namespace engine::reflection
{
    class ReflectionRegistry
    {
      public:
        static ReflectionRegistry& getInstance();

        ReflectionRegistry(const ReflectionRegistry&)            = delete;
        ReflectionRegistry& operator=(const ReflectionRegistry&) = delete;
        ReflectionRegistry(ReflectionRegistry&&)                 = delete;
        ReflectionRegistry& operator=(ReflectionRegistry&&)      = delete;

        void registerAllFromRTTR();

        std::vector<const model::Method*> getAllMethods() const;
        [[nodiscard]] const model::Type* getType(const std::string& name) const;
        [[nodiscard]] const model::Method* getMethod(const std::string& name) const;

        bool hasType(const std::string& name) const;
        bool hasMethod(const std::string& name) const;

      private:
        std::unordered_map<std::string, model::Type> m_types;
        std::unordered_map<std::string, model::Method> m_methods;
        std::unordered_map<std::string, model::Parameter> m_parameters;

        ReflectionRegistry() = default;

        void registerType(const rttr::type& type);
        void registerMethod(const rttr::method& method);
    };
} // namespace engine::reflection