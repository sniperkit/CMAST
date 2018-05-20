#pragma once

#include <istream>

#include <cmast/parser/lexer.hpp>

namespace cmast::lexer {
    auto lex_comment(std::istream& input) -> Comment;
}
