#pragma once

namespace codegen::analysis
{
    class HeaderCollectorException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class HeaderCollector final
    {
    public:
        static void collect_headers_to_file(const fs::path& include_dir,
                                            const fs::path& output_file,
                                            const std::vector<std::string>& extensions = {".h", ".hpp"});

    private:
        HeaderCollector() = default;
    };
} // namespace codegen::analysis
