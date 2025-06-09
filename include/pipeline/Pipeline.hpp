#pragma once

#include "Command.hpp"
#include "pch.hpp"

namespace engine::pipeline
{
    class Pipeline final
    {
      public:
        explicit Pipeline(const nlohmann::json& json);

        void execute();
        void clear();

        std::string_view name() const noexcept
        {
            return name_;
        }

      private:
        std::string name_;
        std::vector<std::unique_ptr<Command>> commands_;
    };
} // namespace engine::pipeline