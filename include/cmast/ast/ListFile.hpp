#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include <cmast/ast/FunctionCall.hpp>

namespace cmast {
    struct ListFile {
        std::string filepath;
        std::vector<FunctionCall> calls;

        friend bool operator==(ListFile const& lhs, ListFile const& rhs)
        {
            return lhs.as_tuple() == rhs.as_tuple();
        }

        friend bool operator!=(ListFile const& lhs, ListFile const& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        auto as_tuple() const -> decltype(std::tie(filepath, calls))
        {
            return std::tie(filepath, calls);
        }
    };

    auto operator<<(std::ostream&, ListFile const&) -> std::ostream&;
}
