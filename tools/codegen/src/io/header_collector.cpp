#include "io/header_collector.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace codegen::io
{
    void HeaderCollector::collect_headers_to_file(const std::vector<std::filesystem::path>& include_dirs,
                                                  const std::filesystem::path& output_file,
                                                  const std::vector<std::string>& extensions)
    {
        const std::unordered_set extension_set(extensions.begin(), extensions.end());

        if (extension_set.empty())
        {
            throw HeaderCollectorException("No file extensions provided for header collection.");
        }

        std::ofstream out(output_file);
        if (!out)
        {
            throw HeaderCollectorException(std::format("Failed to open output file: {}", output_file.string()));
        }

        for (const auto& dir : include_dirs)
        {
            if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
            {
                throw HeaderCollectorException(std::format("Invalid include directory: {}", dir.string()));
            }

            for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
            {
                if (!entry.is_regular_file())
                {
                    continue;
                }

                const auto& file_path = entry.path();
                const std::string extension = file_path.extension().string();

                if (extension_set.contains(extension))
                {
                    const auto relative_path = std::filesystem::relative(file_path, dir);
                    out << "#include \"" << relative_path.generic_string() << "\"\n";
                }
            }
        }
    }
} // namespace codegen::io
