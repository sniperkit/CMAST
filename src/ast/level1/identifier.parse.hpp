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

#include <cmast/ast/level1/identifier.hpp>

#include "parser/level1.hpp"
#include "parser/spirit_x3.hpp"

BOOST_FUSION_ADAPT_STRUCT(cmast::level1::identifier, value);

namespace cmast::detail::level1 {
    inline auto const identifier_impl
        = x3::rule<struct identifier_impl_tag, std::string>(
            "level1::identifier_impl");

    inline auto const identifier_impl_def
        = x3::lexeme[x3::char_("A-Za-z_") >> *x3::char_("A-Za-z0-9_")];

    BOOST_SPIRIT_DEFINE(identifier_impl);

    inline auto const identifier
        = x3::rule<struct identifier_tag, lvl1::identifier>(
            "level1::identifier");

    inline auto const identifier_def = identifier_impl;

    BOOST_SPIRIT_DEFINE(identifier);
}
