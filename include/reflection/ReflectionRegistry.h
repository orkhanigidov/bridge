#pragma once

#include "../model/Class.h"
#include "../model/Method.h"
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
        [[nodiscard]] const model::Class* getClass(const std::string& name) const;
        [[nodiscard]] const model::Method* getMethod(const std::string& name) const;

        bool hasClass(const std::string& name) const;
        bool hasMethod(const std::string& name) const;

      private:
        std::unordered_map<std::string, model::Class> m_classes;
        std::unordered_map<std::string, model::Method> m_methods;

        ReflectionRegistry() = default;

        void registerClass(const rttr::type& type);
        void registerMethod(const rttr::method& method);
        model::Parameter registerClassAsParameter(const std::string& name) const;
    };
} // namespace engine::reflection