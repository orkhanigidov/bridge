#pragma once

#include "pch.hpp"

namespace engine::pipeline
{
    class ObjectPool final
    {
      public:
        static ObjectPool& instance()
        {
            static ObjectPool instance;
            return instance;
        }

        void store(std::string_view alias, rttr::variant object);
        bool has_object(std::string_view alias) const noexcept;
        const rttr::variant& get_object(std::string_view alias) const;
        void update_object(std::string_view alias, const rttr::variant& object);
        void clear_all();

      private:
        std::unordered_map<std::string, rttr::variant> objects_;
    };
} // namespace engine::pipeline