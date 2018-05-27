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

#include <cstddef>
#include <string_view>
#include <tuple>

namespace cmast {
    struct SourceInfo {
        std::string_view filepath;
        std::size_t line;

        friend bool operator==(SourceInfo const& lhs, SourceInfo const& rhs)
        {
            return lhs.as_tuple() == rhs.as_tuple();
        }

        friend bool operator!=(SourceInfo const& lhs, SourceInfo const& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        auto as_tuple() const -> decltype(std::tie(filepath, line))
        {
            return std::tie(filepath, line);
        }
    };

    auto operator<<(std::ostream&, SourceInfo const&) -> std::ostream&;
}
