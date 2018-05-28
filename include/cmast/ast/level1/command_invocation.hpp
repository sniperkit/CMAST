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
#include <vector>

#include <cmast/ast/level1/argument.hpp>

namespace cmast::level1 {
    class command_invocation {
    public:
        command_invocation() = default;

        explicit command_invocation(
            std::string name, std::vector<argument> arguments);

        auto name() const -> std::string const&;

        auto arguments() const -> std::vector<argument> const&;

    private:
        std::string name_;
        std::vector<argument> arguments_;
    };
}
