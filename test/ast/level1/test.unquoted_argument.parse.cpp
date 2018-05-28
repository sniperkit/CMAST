/*

This file is part of CMAST

Copyright (C) 2018  Justin Bassett

CMAST is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CMAST is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ast/level1/unquoted_argument.parse.hpp"

#include <string>
#include <vector>

#include <catch.hpp>

namespace x3 = boost::spirit::x3;

namespace ps = cmast::detail::level1;

TEST_CASE("level1: parse unquoted_argument", "[parser][level1]")
{
    {
        struct TestPair {
            std::string value;
            std::string expected = value;
        };

        auto const validArgs = std::vector<TestPair>({
            TestPair{"someIdentifier"},
            TestPair{"${SomeVariable}"},
            TestPair{"can_have_'in_it"},
            TestPair{R"==(every\ escape_\.sequence\ \(allowed\ \)\"\#_in\\)==",
                R"==(every escape_.sequence (allowed )"#_in\)=="},
        });

        for (auto const& arg : validArgs) {
            SECTION("Valid Arg: `" + arg.value + "`")
            {
                auto result = cmast::level1::unquoted_argument();
                auto first = arg.value.begin();
                auto last = arg.value.end();

                REQUIRE(x3::parse(first, last, ps::unquoted_argument, result));
                CHECK(first == last);
                CHECK(result.value == arg.expected);
            }
        }
    }
    {
        auto const invalidArgs = std::vector<std::string>({
            "something with spaces",
            "\"starts with a quote",
            "\"quoted arg\"",
            "(paren)",
            "Has#comment",
        });

        for (auto const& invalidArg : invalidArgs) {
            SECTION("Invalid Arg: `" + invalidArg + "`")
            {
                auto result = cmast::level1::unquoted_argument();
                auto first = invalidArg.begin();
                auto last = invalidArg.end();

                auto const parsed
                    = x3::parse(first, last, ps::unquoted_argument, result);

                auto const consumed = first == last;

                CAPTURE(parsed);
                CAPTURE(consumed);

                auto const correct = !parsed || !consumed;
                REQUIRE(correct);
            }
        }
    }
}
