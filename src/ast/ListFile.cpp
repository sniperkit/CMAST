#include <cmast/ast/ListFile.hpp>

#include <ostream>

namespace cmast {
    auto operator<<(std::ostream& out, ListFile const& file) -> std::ostream&
    {
        out << "ListFile: " << file.filepath << " {\n";

        for (auto const& call : file.calls) {
            out << "  " << call << ",\n";
        }

        return out << "}";
    }
}
