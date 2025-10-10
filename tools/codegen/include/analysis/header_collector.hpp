#pragma once

namespace codegen::analysis {

    class HeaderCollector final {
    public:
        HeaderCollector() = default;

        bool collect_headers(const fs::path& include_dir, const fs::path& output_file);

    private:
        static bool is_header_file(const fs::path& path);
    };

} // namespace codegen::analysis
