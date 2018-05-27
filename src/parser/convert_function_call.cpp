#include "convert_function_call.hpp"

#include "convert_function_argument.hpp"
#include "convert_source_info.hpp"

namespace cmast::detail {
    auto convert_function_call(std::string_view filepath,
        cmListFileFunction const& functionCall) -> FunctionCall
    {
        auto result = FunctionCall();

        result.source_info
            = detail::convert_source_info(filepath, functionCall.Line);
        result.name = functionCall.Name.Original;

        result.arguments.reserve(functionCall.Arguments.size());
        for (auto const& arg : functionCall.Arguments) {
            result.arguments.push_back(
                detail::convert_function_argument(filepath, arg));
        }

        return result;
    }
}
