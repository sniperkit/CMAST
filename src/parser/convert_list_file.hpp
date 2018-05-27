#pragma once

#include <string>

#include <cmListFileCache.h>

#include <cmast/ast/ListFile.hpp>

namespace cmast::detail {
    auto convert_list_file(std::string filepath, cmListFile const& listFile)
        -> ListFile;
}
