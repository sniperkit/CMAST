#include <cmast/ast/SourceInfo.hpp>

#include <ostream>

namespace cmast {
    auto operator<<(std::ostream& out, SourceInfo const& argument)
        -> std::ostream&
    {
        return out << "at `" << argument.filepath << "`:" << argument.line;
    }
}
