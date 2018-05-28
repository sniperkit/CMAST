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

#pragma once

#include <string>

#include "parser/level1.hpp"
#include "parser/spirit_x3.hpp"

namespace cmast::detail::level1 {
    inline auto const escape_identity
        = x3::lexeme[x3::lit('\\') >> (x3::char_ - x3::char_("A-Za-z0-9;"))];

    inline auto const escape_encoded = (x3::lit("\\t") >> x3::attr('\t'))
        | (x3::lit("\\r") >> x3::attr('\r'))
        | (x3::lit("\\n") >> x3::attr('\n'));

    inline auto const escape_semicolon = x3::lit('\\') >> x3::char_(';');

    inline auto const escape_sequence
        = escape_identity | escape_encoded | escape_semicolon;
}
