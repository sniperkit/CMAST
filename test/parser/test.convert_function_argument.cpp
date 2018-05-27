#include "parser/convert_function_argument.hpp"

#include <cmListFileCache.h>

#include <catch.hpp>

TEST_CASE("parser/convert_function_argument", "[parser]")
{
    auto const cmArgument = cmListFileArgument(
        "41ad8a42-515b-49ac-b606-dbdc2a587db6", cmListFileArgument::Quoted, 58);

    auto const argument
        = cmast::detail::convert_function_argument("filepath", cmArgument);

    CHECK(argument
        == cmast::FunctionArgument{cmast::SourceInfo{"filepath", 58},
               "41ad8a42-515b-49ac-b606-dbdc2a587db6",
               cmast::FunctionArgument::Quoted});
}
