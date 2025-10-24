#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

namespace codegen::io
{
    class HeaderCollectorException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class HeaderCollector final
    {
    public:
        static void collect_headers_to_file(const std::vector<std::filesystem::path>& include_dirs,
                                            const std::filesystem::path& output_file,
                                            const std::vector<std::string>& extensions = {".h", ".hpp"});

    private:
        HeaderCollector() = default;
    };
} // namespace codegen::io
