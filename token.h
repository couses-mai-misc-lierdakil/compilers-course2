#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant>
#include <memory>
#include <optional>

enum class TokenType {
    Var, Num, Op, Eof
};

class Token
{
public:
    const TokenType type;
    Token(TokenType type);
    virtual ~Token();
    virtual std::wstring toString() = 0;
};

class TokenNum : public Token {
public:
    using value_t = double;
    const value_t value;
    TokenNum(double value);
    std::wstring toString();
};

class TokenVar : public Token {
public:
    using value_t = std::wstring;
    const value_t value;
    TokenVar(std::wstring name);
    std::wstring toString();
};

class TokenOp : public Token {
public:
    using value_t = wchar_t;
    const value_t value;
    TokenOp(wchar_t op);
    std::wstring toString();
};

class TokenEof : public Token {
public:
    using value_t = int;
    const value_t value;
    TokenEof();
    std::wstring toString();
};

// helpers

template <TokenType tt>
struct Token_sub_t{
    using sub_t = std::nullptr_t;
};

#define TOK_SUB_T(t) \
    template <> struct Token_sub_t<TokenType::t>{ using sub_t = Token ## t; };

TOK_SUB_T(Var)
TOK_SUB_T(Op)
TOK_SUB_T(Num)
TOK_SUB_T(Eof)

template<TokenType tt>
std::optional<typename Token_sub_t<tt>::sub_t::value_t> getToken(
        const std::unique_ptr<Token>& t) {
    if (t->type != tt) return std::nullopt;
    else {
        auto ptr = dynamic_cast<typename Token_sub_t<tt>::sub_t*>(t.get());
        if (ptr) return ptr->value;
        else return std::nullopt;
    }
}

#endif // TOKEN_H
