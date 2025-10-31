/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <filesystem>
#include <system_error>
#include <utility>

namespace engine::utils
{
    class TempFileGuard final
    {
    public:
        explicit TempFileGuard(std::filesystem::path path): path_(std::move(path))
        {
        }

        ~TempFileGuard()
        {
            std::error_code ec;
            if (std::filesystem::exists(path_, ec))
            {
                std::filesystem::remove(path_, ec);
            }
        }

    private:
        std::filesystem::path path_;
    };
} // namespace engine::utils
