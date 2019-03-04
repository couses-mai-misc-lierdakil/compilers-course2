#include "token.h"
#include <sstream>

Token::Token(TokenType type): type(type) {}

Token::~Token() {}

TokenNum::TokenNum(double value): Token(TokenType::Num), value(value) {}

std::wstring TokenNum::toString() {
    std::wstringstream str;
    str << "Token Number value = " << value;
    return str.str();
}

TokenVar::TokenVar(std::wstring name) : Token(TokenType::Var), value(name) {}

std::wstring TokenVar::toString() {
    std::wstringstream str;
    str << L"Token Variable name = " << value;
    return str.str();
}

TokenOp::TokenOp(wchar_t op) : Token(TokenType::Op), value(op) {}

std::wstring TokenOp::toString() {
    std::wstringstream str;
    str << "Token Operator type = " << value;
    return str.str();
}

TokenEof::TokenEof() : Token(TokenType::Eof), value(EOF) {}

std::wstring TokenEof::toString() {
    std::wstringstream str;
    str << L"Token EOF";
    return str.str();
}
