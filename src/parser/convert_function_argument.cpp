#include "convert_function_argument.hpp"

#include "convert_source_info.hpp"

namespace cmast::detail {
    namespace {
        auto convert_delimiter(cmListFileArgument::Delimiter delim)
            -> FunctionArgument::Delimiter
        {
            return FunctionArgument::Delimiter(delim);
        }
    }

    auto convert_function_argument(std::string_view filepath,
        cmListFileArgument const& argument) -> FunctionArgument
    {
        auto result = FunctionArgument();

        result.source_info
            = detail::convert_source_info(filepath, argument.Line);
        result.value = argument.Value;
        result.delimiter = detail::convert_delimiter(argument.Delim);

        return result;
    }
}
