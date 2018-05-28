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

#include "ast/level1/escape_sequence.parse.hpp"

#include <string>
#include <vector>

#include <catch.hpp>

namespace x3 = boost::spirit::x3;

namespace ps = cmast::detail::level1;

TEST_CASE("level1: parse escape_sequence", "[parser][level1]")
{
    {
        struct EscapePair {
            std::string value;
            std::string expected;
        };

        auto const validEscapeSequences = std::vector<EscapePair>({
            EscapePair{"\\t", "\t"},
            EscapePair{"\\n", "\n"},
            EscapePair{"\\r", "\r"},
            EscapePair{"\\;", ";"},
            EscapePair{"\\(", "("},
            EscapePair{"\\?", "?"},
        });

        for (auto const& es : validEscapeSequences) {
            SECTION("Valid Escape: " + es.value)
            {
                auto result = std::string();
                auto first = es.value.begin();
                auto last = es.value.end();

                REQUIRE(x3::parse(first, last, ps::escape_sequence, result));
                CHECK(first == last);
                CHECK(result == es.expected);
            }
        }
    }
    {
        auto const invalidEscapes = std::vector<std::string>({
            "\\s",
        });

        for (auto const& invalidEsc : invalidEscapes) {
            SECTION("Invalid Escape: " + invalidEsc)
            {
                auto result = std::string();
                auto first = invalidEsc.begin();
                auto last = invalidEsc.end();

                auto const parsed
                    = x3::parse(first, last, ps::escape_sequence, result);

                auto const consumed = first == last;

                CAPTURE(parsed);
                CAPTURE(consumed);

                auto const correct = !parsed || !consumed;
                REQUIRE(correct);
            }
        }
    }
}
