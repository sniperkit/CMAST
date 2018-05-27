#include "convert_source_info.hpp"

#include <ostream>
#include <utility>

#include <cmast/ast/SourceInfo.hpp>

namespace cmast::detail {
    auto convert_source_info(std::string_view filepath, long line) -> SourceInfo
    {
        auto result = SourceInfo();

        result.filepath = std::move(filepath);
        result.line = line;

        return result;
    }
}
