#pragma once

#include "../pch.h"

namespace engine::operation
{
    class ObjectStore final
    {
      public:
        static ObjectStore& getInstance();

        ObjectStore(const ObjectStore&)            = delete;
        ObjectStore& operator=(const ObjectStore&) = delete;
        ObjectStore(ObjectStore&&)                 = delete;
        ObjectStore& operator=(ObjectStore&&)      = delete;

        void storeObject(const std::string& id, const rttr::type& type);
        bool hasObject(const std::string& id);
        void removeObject(const std::string& id);
        const rttr::variant& getObject(const std::string& id);
        void updateObject(const std::string& id, const rttr::variant& variant);
        void clearAll();

      private:
        std::unordered_map<std::string, rttr::variant> m_references;

        ObjectStore() = default;
    };
} // namespace engine::operation