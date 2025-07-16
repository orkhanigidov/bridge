#include "codegen/header_includer.hpp"

namespace codegen
{
    HeaderIncluder::HeaderIncluder(const std::string& include_dir, const std::string& output_filename)
    {
        const std::filesystem::path include_dir_path(include_dir);

        if (!std::filesystem::exists(include_dir_path) || !std::filesystem::is_directory(include_dir_path))
        {
            std::cerr << "Invalid include directory: " << include_dir_path << std::endl;
            return;
        }

        std::ofstream out(output_filename);
        if (!out)
        {
            std::cerr << "Failed to open output file: " << output_filename << std::endl;
            return;
        }

        for (const auto& entry : std::filesystem::recursive_directory_iterator(include_dir_path))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            if (const std::filesystem::path& file_path = entry.path(); isHeaderFile(file_path))
            {
                out << "#include \"" << file_path.string() << "\"\n";
            }
        }

        std::cout << "Header includes written to: " << output_filename << std::endl;
    }

    bool HeaderIncluder::isHeaderFile(const std::filesystem::path& path)
    {
        return path.extension() == ".h" || path.extension() == ".hpp";
    }
} // namespace codegen