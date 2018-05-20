/*

This file is part of cmast

Copyright (C) 2018  Justin Bassett

cmast is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

cmast is distributed in the hope that it will be useful,
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

TEST_CASE("lexing comments", "[parser]")
{
    auto token = lexer::Token();

    SECTION("comparisons")
    {
        CHECK(lexer::Comment() == lexer::Comment());
        CHECK(lexer::Token(lexer::Comment()) == lexer::Comment());
        CHECK(lexer::Comment() == lexer::Token(lexer::Comment()));
        CHECK(lexer::Token(lexer::Comment()) == lexer::Token(lexer::Comment()));

        CHECK_FALSE(lexer::Comment() != lexer::Comment());
        CHECK_FALSE(lexer::Token(lexer::Comment()) != lexer::Comment());
        CHECK_FALSE(lexer::Comment() != lexer::Token(lexer::Comment()));
        CHECK_FALSE(lexer::Token(lexer::Comment()) != lexer::Token(lexer::Comment()));
    }

    SECTION("line comments")
    {
        SECTION("empty comment")
        {
            SECTION("pound newline")
            {
                auto input = std::istringstream("#\n");
                CHECK(input >> token);
                CHECK(token == lexer::Comment(""));
            }
            SECTION("pound EOF")
            {
                std::istringstream input("#");
                input >> token;
                CHECK(input.eof());
                CHECK_FALSE(input.bad());
                CHECK(token == lexer::Comment(""));
            }
        }
        SECTION("no leading whitespace, ends in newline")
        {
            auto const commentBody = std::string("eb104698-5260-4a76-9eaa-da1281d3830f");

            SECTION("no whitespace after comment begin")
            {
                auto input = std::istringstream("#" + commentBody + "\n");
                CHECK(input >> token);
                CHECK(token == lexer::Comment(commentBody));
            }
            SECTION("whitespace after comment begin")
            {
                auto input = std::istringstream("#\t" + commentBody + "\n");
                CHECK(input >> token);
                CHECK(token == lexer::Comment("\t" + commentBody));
            }
        }
        SECTION("leading whitespace")
        {
            auto const expected = std::string("wheyishness medisection restrainedness antimonopolist laryngitic");

            auto input = std::istringstream("  #" + expected + "\n");
            CHECK(input >> token);
            CHECK(token == lexer::Comment(expected));
        }
        SECTION("trailing whitespace")
        {
            auto const expected = std::string("tetrasulphide prelabor upswallow \t");

            auto input = std::istringstream("#" + expected + "\n");
            CHECK(input >> token);
            CHECK(token == lexer::Comment(expected));
        }
        SECTION("Tricky comments")
        {
            SECTION("block-like newline") {
                auto input = std::istringstream("#[\n");
                CHECK(input >> token);
                CHECK(token == lexer::Comment("["));
            }
            SECTION("block-like EOF") {
                auto input = std::istringstream("#[");
                input >> token;
                CHECK(input.eof());
                CHECK_FALSE(input.bad());
                CHECK(token == lexer::Comment("["));
            }
        }
    }

    SECTION("block comments")
    {
        SECTION("on a single line")
        {
            auto const expected = std::string("some text");
            auto input = std::istringstream("#[[" + expected + "]]");

            CHECK(input >> token);
            CHECK(token == lexer::Comment(expected));
        }
        SECTION("on multiple lines")
        {
            auto const expected = std::string(R"comment(
                some text in between
                block comment delimeters
                #[[
                #
                ]
                still some more
            )comment");

            auto input = std::istringstream("#[[" + expected + "]]");

            CHECK(input >> token);
            CHECK(token == lexer::Comment(expected));
        }
    }
}
