#pragma once

#include <string_view>

#include <cmListFileCache.h>

#include <cmast/ast/FunctionCall.hpp>

namespace cmast::detail {
    auto convert_function_call(std::string_view filepath,
        cmListFileFunction const& functionCall) -> FunctionCall;
}
