#pragma once

#include <filesystem>

#include <cmast/ast.hpp>

namespace cmast {
    auto parse_list_file(std::filesystem::path const& filepath) -> ListFile;
}
