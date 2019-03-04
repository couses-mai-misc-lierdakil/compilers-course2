#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <memory>
#include "token.h"

class Lexer;

class Parser {
public:
    Parser(Lexer &lexer);
    double parse();

private:
    Lexer &l;
    std::unique_ptr<Token> currentTok;
    std::map<std::wstring, double> symtable;

    double parseV();
    double parseF();
    double parseTprime(double op1val);
    double parseT();
    double parseEprime(double op1val);
    double parseE();

    void nextTok();
    template<TokenType tt>
    auto curTokVal() { return getToken<tt>(currentTok); }
};


#endif // PARSER_H
