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
#include <istream>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>

namespace cmast::lexer {
    namespace detail {
        struct TokenBase {
            template <typename Lhs, typename Rhs, typename = std::enable_if_t<!std::is_same_v<Lhs, Rhs>>>
            friend bool operator==(Lhs const&, Rhs const&) { return false; }

            template <typename Lhs, typename Rhs, typename = std::enable_if_t<!std::is_same_v<Lhs, Rhs>>>
            friend bool operator!=(Lhs const&, Rhs const&) { return true; }
        };

        template <typename Derived>
        struct TokenEq : TokenBase {
            template <typename... Ts>
            friend bool operator==(std::variant<Ts...> const& lhs, Derived const& rhs)
            {
                auto const* it = std::get_if<Derived>(&lhs);
                return it != nullptr && *it == rhs;
            }

            template <typename... Ts>
            friend bool operator==(Derived const& lhs, std::variant<Ts...> const& rhs)
            {
                return rhs == lhs;
            }

            template <typename... Ts>
            friend bool operator!=(std::variant<Ts...> const& lhs, Derived const& rhs)
            {
                auto const* it = std::get_if<Derived>(&lhs);
                return it != nullptr && *it != rhs;
            }

            template <typename... Ts>
            friend bool operator!=(Derived const& lhs, std::variant<Ts...> const& rhs)
            {
                return rhs != lhs;
            }
        };

        template <typename Derived>
        struct StrongToken : private TokenEq<Derived> {
            friend bool operator==(Derived const& lhs, Derived const& rhs)
            {
                return lhs.get_underlying() == rhs.get_underlying();
            }

            friend bool operator!=(Derived const& lhs, Derived const& rhs)
            {
                return lhs.get_underlying() != rhs.get_underlying();
            }

            friend auto operator<<(std::ostream& out, Derived const& val) -> std::ostream&
            {
                return out << Derived::TypeName << "(\"" << val.get_underlying() << "\")";
            }
        };

        template <typename Derived>
        struct StatelessToken : private TokenEq<Derived> {
            friend bool operator==(Derived const& lhs, Derived const& rhs)
            {
                return true;
            }

            friend bool operator!=(Derived const& lhs, Derived const& rhs)
            {
                return false;
            }

            friend auto operator<<(std::ostream& out, Derived const& val) -> std::ostream&
            {
                return out << Derived::TypeName;
            }
        };
    }

    class Comment : detail::StrongToken<Comment> {
    public:
        Comment() = default;

        explicit Comment(std::string value)
            : value_(std::move(value))
        {
        }

        void push_back(char c)
        {
            value_.push_back(c);
        }

    private:
        std::string value_;

        friend class detail::StrongToken<Comment>;

        auto const& get_underlying() const { return value_; }

        static constexpr std::string_view TypeName = "Comment";
    };

    class Identifier : detail::StrongToken<Identifier> {
    public:
        Identifier() = default;

        explicit Identifier(std::string value)
            : value_(std::move(value))
        {
        }

        void push_back(char c)
        {
            value_.push_back(c);
        }

    private:
        std::string value_;

        friend class detail::StrongToken<Identifier>;

        auto const& get_underlying() const { return value_; }

        static constexpr auto TypeName = std::string_view("Identifier");
    };

    class OpenParen : detail::StatelessToken<OpenParen> {
        friend class detail::StatelessToken<OpenParen>;
        static constexpr auto TypeName = std::string_view("OpenParen");
    };

    class CloseParen : detail::StatelessToken<CloseParen> {
        friend class detail::StatelessToken<CloseParen>;
        static constexpr auto TypeName = std::string_view("CloseParen");
    };

    class GeneratorExpressionStart : detail::StatelessToken<GeneratorExpressionStart> {
        friend class detail::StatelessToken<GeneratorExpressionStart>;
        static constexpr auto TypeName = std::string_view("GeneratorExpressionStart");
    };

    class GeneratorExpressionEnd : detail::StatelessToken<GeneratorExpressionEnd> {
        friend class detail::StatelessToken<GeneratorExpressionEnd>;
        static constexpr auto TypeName = std::string_view("GeneratorExpressionEnd");
    };

    class VariableReferenceStart : detail::StatelessToken<VariableReferenceStart> {
        friend class detail::StatelessToken<VariableReferenceStart>;
        static constexpr auto TypeName = std::string_view("VariableReferenceStart");
    };

    class VariableReferenceEnd : detail::StatelessToken<VariableReferenceEnd> {
        friend class detail::StatelessToken<VariableReferenceEnd>;
        static constexpr auto TypeName = std::string_view("VariableReferenceEnd");
    };

    using Token = std::variant<
        std::monostate,
        Comment,
        Identifier,
        OpenParen,
        CloseParen,
        GeneratorExpressionStart,
        GeneratorExpressionEnd,
        VariableReferenceStart,
        VariableReferenceEnd>;

    inline auto operator<<(std::ostream& out, Token const& token) -> std::ostream&
    {
        std::visit([&](auto const& it) {
            if constexpr (std::is_same_v<std::decay_t<decltype(it)>, std::monostate>) {
                out << "<>";
            } else {
                out << '<' << it << '>';
            }
        },
            token);

        return out;
    }

    auto operator>>(std::istream&, Token& token) -> std::istream&;
}
