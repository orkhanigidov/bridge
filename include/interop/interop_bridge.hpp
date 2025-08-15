#pragma once

namespace interop {
    bool execute_script(const fs::path& script_path,
                        const fs::path& input_data_path,
                        const fs::path& output_data_path);
} // namespace interop
