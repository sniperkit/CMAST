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

#include <iosfwd>
#include <string>
#include <tuple>

#include <cmast/ast/SourceInfo.hpp>

namespace cmast {
    struct FunctionArgument {
        enum Delimiter {
            Unquoted,
            Quoted,
            Bracketed,
        };

        SourceInfo source_info;
        std::string value;
        Delimiter delimiter;

        friend bool operator==(
            FunctionArgument const& lhs, FunctionArgument const& rhs)
        {
            return lhs.as_tuple() == rhs.as_tuple();
        }

        friend bool operator!=(
            FunctionArgument const& lhs, FunctionArgument const& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        auto as_tuple() const
            -> decltype(std::tie(source_info, value, delimiter))
        {
            return std::tie(source_info, value, delimiter);
        }
    };

    auto operator<<(std::ostream&, FunctionArgument const&) -> std::ostream&;
}
