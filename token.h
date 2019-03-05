#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant>
#include <optional>

namespace Tok {

#define TOK(tokName, valueType) struct tokName { \
    constexpr static auto name = #tokName;\
    valueType value;\
};

TOK(Var, std::wstring)
TOK(Num, double)
TOK(Op, wchar_t)
TOK(Eof, std::monostate)

#undef TOK

using Token = std::variant<Var, Num, Op, Eof>;

template<typename T>
std::optional<decltype(T::value)> getValue(const Token& t) {
    if(auto val = std::get_if<T>(&t)) return val->value;
    else return std::nullopt;
}

std::wstring toString(const Token&);

std::wostream& operator<<(std::wostream& s, const Token& tok);

}

#endif // TOKEN_H
