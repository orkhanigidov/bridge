#pragma once

#include "../pch.h"

#include "ObjectNode.h"

namespace engine::operation
{
    class ObjectPool final
    {
      public:
        static ObjectPool& instance();

        void                 store(std::string_view id, std::unique_ptr<ObjectNode> object);
        bool                 has_object(std::string_view id);
        const rttr::variant& get_object(std::string_view id);
        bool                 remove(std::string_view id);
        void                 clear_all();
        // void                 update(std::string_view id, const rttr::variant& variant);

      private:
        std::unordered_map<std::string, std::unique_ptr<ObjectNode>> objects_;

        ObjectPool() = default;
    };
} // namespace engine::operation