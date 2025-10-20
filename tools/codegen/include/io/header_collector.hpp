#pragma once

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
        static void collect_headers_to_file(const std::vector<fs::path>& include_dirs,
                                            const fs::path& output_file,
                                            const std::vector<std::string>& extensions = {".h", ".hpp"});

    private:
        HeaderCollector() = default;
    };
} // namespace codegen::io
