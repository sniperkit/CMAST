#include "identifiers.hpp"

#include <string>
#include <utility>

namespace cmast::lexer {
    namespace {
        auto is_identifier_continue(char c) -> bool
        {
            return ('A' <= c && c <= 'Z')
                || ('a' <= c && c <= 'z')
                || ('0' <= c && c <= '9')
                || c == '_';
        }
    }

    auto lex_identifier(std::istream& input) -> Identifier
    {
        auto result = std::string();

        do {
            result.push_back(input.get());
        } while (is_identifier_continue(input.peek()));

        return Identifier(std::move(result));
    }
}
