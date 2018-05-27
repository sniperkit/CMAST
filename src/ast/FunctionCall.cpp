#include <cmast/ast/FunctionCall.hpp>

#include <ostream>

namespace cmast {

    auto operator<<(std::ostream& out, FunctionCall const& call)
        -> std::ostream&
    {
        out << "\"" << call.name << "\"(";

        for (auto const& arg : call.arguments) {
            out << arg << ", ";
        }

        return out << ") " << call.source_info;
    }
}
