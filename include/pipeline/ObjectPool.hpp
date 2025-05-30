#pragma once

#include "pch.hpp"

namespace engine::pipeline
{
    class ObjectPool final
    {
      public:
        static ObjectPool& instance();

        void store(std::string_view id, rttr::variant object);
        bool has_object(std::string_view id) const noexcept;
        const rttr::variant& get_object(std::string_view id) const;
        bool remove(std::string_view id);
        void clear_all();

        rttr::variant resolve_reference(std::string_view reference) const;

      private:
        std::unordered_map<std::string, rttr::variant> objects_;

        ObjectPool() = default;
    };
} // namespace engine::pipeline