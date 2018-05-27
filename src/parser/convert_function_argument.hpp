#pragma once

#include <string_view>

#include <cmListFileCache.h>

#include <cmast/ast/FunctionArgument.hpp>

namespace cmast::detail {
    auto convert_function_argument(std::string_view filepath,
        cmListFileArgument const& argument) -> FunctionArgument;
}
