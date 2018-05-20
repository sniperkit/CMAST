#include "comments.hpp"

#include <string>
#include <utility>

namespace cmast::lexer {
    namespace {
        auto lex_block_comment(std::istream& input) -> Comment
        {
            auto result = std::string();

            auto count = int();
            do {
                auto c = input.get();
                if (c == ']') {
                    count++;
                } else {
                    count = 0;
                }

                result.push_back(c);
            } while (input && count < 2);

            result.pop_back();
            result.pop_back();

            return Comment(result);
        }
    }

    auto lex_comment(std::istream& input) -> Comment
    {
        auto result = std::string();

        input.get(); // remove '#'

        if (input.peek() == '[') {
            input.get();
            auto c = input.get();
            if (!input || c == '\n') return Comment("[");
            if (c == '[') {
                return lexer::lex_block_comment(input);
            } else {
                result.push_back('[');
                result.push_back(c);
            }
        }

        std::getline(input, result);

        return Comment(std::move(result));
    }
}
