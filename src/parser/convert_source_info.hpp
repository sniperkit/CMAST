#pragma once

#include <string_view>

#include <cmast/ast/SourceInfo.hpp>

namespace cmast::detail {
    auto convert_source_info(std::string_view filepath, long line)
        -> SourceInfo;
}
