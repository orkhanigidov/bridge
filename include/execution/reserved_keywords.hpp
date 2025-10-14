#pragma once

namespace engine::execution
{
    class Reserved final
    {
    public:
        Reserved() = delete;

        static constexpr auto INPUT_PATH = "__input__";
        static constexpr auto OUTPUT_PATH = "__output__";
    };
} // namespace engine::execution
