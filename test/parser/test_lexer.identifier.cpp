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

#include <cmast/parser/lexer.hpp>

#include <sstream>
#include <vector>

#include <catch.hpp>

namespace lexer = cmast::lexer;

TEST_CASE("lexing identifiers", "[parser]")
{
    auto token = lexer::Token();

    SECTION("comparisons")
    {
        CHECK(lexer::Identifier() == lexer::Identifier());
        CHECK(lexer::Token(lexer::Identifier()) == lexer::Identifier());
        CHECK(lexer::Identifier() == lexer::Token(lexer::Identifier()));
        CHECK(lexer::Token(lexer::Identifier()) == lexer::Token(lexer::Identifier()));

        CHECK_FALSE(lexer::Identifier() != lexer::Identifier());
        CHECK_FALSE(lexer::Token(lexer::Identifier()) != lexer::Identifier());
        CHECK_FALSE(lexer::Identifier() != lexer::Token(lexer::Identifier()));
        CHECK_FALSE(lexer::Token(lexer::Identifier()) != lexer::Token(lexer::Identifier()));
    }

    SECTION("valid identifiers")
    {
        auto const identifiers = std::vector<std::string>({
            "anIdentifier",
            "AnIdentifier",
            "_anIdentifier",
            "__a_n_I_d_e_n_t__i_f_i_e_r___",
            "_04536",
        });

        for (auto const& identifier : identifiers) {
            SECTION(identifier)
            {
                auto input = std::istringstream(identifier);
                input >> token;
                CHECK(token == lexer::Identifier(identifier));
            }
        }
    }
}
