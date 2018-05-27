#include <cmast/ast/FunctionArgument.hpp>

#include <ostream>

namespace cmast {
    namespace {
        auto operator<<(std::ostream& out, FunctionArgument::Delimiter delim)
            -> std::ostream&
        {
            switch (delim) {
            case FunctionArgument::Unquoted:
                return out << "Unquoted";
            case FunctionArgument::Quoted:
                return out << "Quoted";
            case FunctionArgument::Bracketed:
                return out << "Bracketed";
            }

            return out;
        }
    }

    auto operator<<(std::ostream& out, FunctionArgument const& argument)
        -> std::ostream&
    {
        return out << "FunctionArgument(.value=\"" << argument.value
                   << "\", .delimiter=" << argument.delimiter << ", "
                   << argument.source_info << ")";
    }
}
