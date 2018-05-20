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

#include <cctype>

#include "lexer/comments.hpp"
#include "lexer/identifiers.hpp"

namespace cmast::lexer {
    auto operator>>(std::istream& input, Token& token) -> std::istream&
    {
        while (std::isspace(input.peek())) {
            input.get();
        }

        auto nextChar = input.peek();

        token = [&]() -> Token {
            if (('A' <= nextChar && nextChar <= 'Z')
                || ('a' <= nextChar && nextChar <= 'z')
                || nextChar == '_') {
                // Does this assume ASCII?

                return lexer::lex_identifier(input);
            }

            if (nextChar == '#') {
                return lexer::lex_comment(input);
            }

            input.get();

            switch (nextChar) {
            case '(':
                return OpenParen();
            case ')':
                return CloseParen();
            }
            // TODO: fix this
            return std::monostate();
        }();

        return input;
    }
}
