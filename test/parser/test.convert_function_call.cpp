#include "parser/convert_function_call.hpp"

#include <cmListFileCache.h>

#include <catch.hpp>

using namespace cmast;

TEST_CASE("parser/convert_function_call", "[parser]")
{
    auto const cmCall = [] {
        auto result = cmListFileFunction();

        result.Name = "Janet";
        result.Line = 54;
        result.Arguments = [] {
            auto result = std::vector<cmListFileArgument>();

            result.emplace_back("Bhutan", cmListFileArgument::Unquoted, 59);

            return result;
        }();

        return result;
    }();

    auto const call = [] {
        auto result = FunctionCall();

        result.source_info = SourceInfo{"a/file/path", 54};
        result.name = "Janet";
        result.arguments.push_back(
            FunctionArgument{SourceInfo{"a/file/path", 59}, "Bhutan",
                FunctionArgument::Unquoted});

        return result;
    }();

    CHECK(call == detail::convert_function_call("a/file/path", cmCall));
}
