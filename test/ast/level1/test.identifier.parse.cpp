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

#include "ast/level1/identifier.parse.hpp"

#include <string>
#include <vector>

#include <catch.hpp>

namespace x3 = boost::spirit::x3;

namespace ps = cmast::detail::level1;

TEST_CASE("level1: parse identifier", "[parser][level1]")
{
    auto const validIdentifiers = std::vector<std::string>({
        "someIdentifier",
    });

    for (auto const& id : validIdentifiers) {
        SECTION("Valid Identifier: " + id)
        {
            auto result = cmast::level1::identifier();
            auto first = id.begin();
            auto last = id.end();

            REQUIRE(x3::parse(first, last, ps::identifier, result));
            CHECK(first == last);
            CHECK(result.value == id);
        }
    }
}
