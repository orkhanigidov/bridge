#include "header_collector.hpp"

namespace codegen {

    bool HeaderCollector::collect_headers(const fs::path& include_dir, const fs::path& output_file)
    {
        if (!fs::exists(include_dir) || !fs::is_directory(include_dir)) {
            std::cerr << "Invalid include directory: " << include_dir << std::endl;
            return false;
        }

        std::ofstream out(output_file);
        if (!out) {
            std::cerr << "Failed to open output file: " << output_file << std::endl;
            return false;
        }

        for (const auto& entry: fs::recursive_directory_iterator(include_dir)) {
            if (!entry.is_regular_file())
            {
                continue;
            }

            if (const auto& file_path = entry.path(); is_header_file(file_path)) {
                const auto relative_path = fs::relative(file_path, include_dir);
                out << "#include \"" << relative_path.generic_string() << "\"\n";
            }
        }

        std::cout << "Header files collected successfully into: " << output_file << std::endl;
        return true;
    }

    bool HeaderCollector::is_header_file(const fs::path& path)
    {
        return path.extension() == ".h" || path.extension() == ".hpp";
    }

} // namespace codegen
