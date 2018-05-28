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

#include <boost/fusion/include/adapt_struct.hpp>

#include <cmast/ast/level1/unquoted_argument.hpp>

#include "parser/level1.hpp"
#include "parser/spirit_x3.hpp"

#include "escape_sequence.parse.hpp"

BOOST_FUSION_ADAPT_STRUCT(cmast::level1::unquoted_argument, value);

namespace cmast::detail::level1 {
    inline auto const unquoted_argument_impl
        = x3::rule<struct unquoted_argument_impl_tag, std::string>(
            "level1::unquoted_argument_impl");

    inline auto const unquoted_argument_impl_def = [] {
        auto const unquoted_char = x3::char_ - x3::char_(R"escapes(()#\")escapes") - x3::space;

        return x3::lexeme[+(unquoted_char | escape_sequence)];
    }();

    BOOST_SPIRIT_DEFINE(unquoted_argument_impl);

    inline auto const unquoted_argument
        = x3::rule<struct unquoted_argument_tag, lvl1::unquoted_argument>(
            "level1::unquoted_argument");

    inline auto const unquoted_argument_def = unquoted_argument_impl;

    BOOST_SPIRIT_DEFINE(unquoted_argument);
}
