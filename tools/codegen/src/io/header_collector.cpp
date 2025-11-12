/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file header_collector.cpp
 * @brief Implements the HeaderCollector for collecting header files from directories.
 */

#include "io/header_collector.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace codegen::io
{
    /**
     * @brief Collects header files from the given include directories and writes their paths to an output file.
     * @param include_dirs The directories to search for header files.
     * @param output_file The file to write the collected header paths to.
     * @param extensions The file extensions to consider as headers.
     * @throws HeaderCollectorException if an error occurs during collection or writing.
     */
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
            throw HeaderCollectorException("Failed to open output file: " + output_file.string());
        }

        for (const auto& dir : include_dirs)
        {
            if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
            {
                throw HeaderCollectorException("Invalid include directory: " + dir.string());
            }

            for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
            {
                if (!entry.is_regular_file())
                {
                    continue;
                }

                const auto& file_path = entry.path();
                const std::string extension = file_path.extension().string();

                if (extension_set.find(extension) != extension_set.end())
                {
                    const auto relative_path = std::filesystem::relative(file_path, dir);
                    out << "#include \"" << relative_path.generic_string() << "\"\n";
                }
            }
        }
    }
} // namespace codegen::io
