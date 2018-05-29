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

#include <variant>

#include <cmast/ast/level1/bracket_argument.hpp>
#include <cmast/ast/level1/quoted_argument.hpp>
#include <cmast/ast/level1/unquoted_argument.hpp>

namespace cmast::level1 {
    class argument {
        using variant_type = std::variant<bracket_argument, quoted_argument,
            unquoted_argument>;

    public:
        argument() = default;

        explicit argument(variant_type value);

        auto value() const -> variant_type const&;

    private:
        variant_type value_;
    };
}