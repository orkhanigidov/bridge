#include "../../include/io/header_collector.hpp"

namespace codegen::analysis
{
    void HeaderCollector::collect_headers_to_file(const fs::path& include_dir,
                                                  const fs::path& output_file,
                                                  const std::vector<std::string>& extensions)
    {
        if (!fs::exists(include_dir) || !fs::is_directory(include_dir))
        {
            throw HeaderCollectorException("Invalid include directory: " + include_dir.string());
        }

        std::ofstream out(output_file);
        if (!out)
        {
            throw HeaderCollectorException("Failed to open output file: " + output_file.string());
        }

        for (const auto& entry : fs::recursive_directory_iterator(include_dir))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            const auto& file_path = entry.path();
            const std::string extension = file_path.extension().string();

            bool is_header = false;
            for (const auto& ext : extensions)
            {
                if (extension == ext)
                {
                    is_header = true;
                    break;
                }
            }

            if (is_header)
            {
                const auto relative_path = fs::relative(file_path, include_dir);
                out << "#include \"" << relative_path.generic_string() << "\"\n";
            }
        }

        std::cout << "Header files collected successfully into: " << output_file << std::endl;
    }
} // namespace codegen::analysis
