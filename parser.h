#ifndef PARSER_H
#define PARSER_H

#include <map>
#include "token.h"

class Lexer;

class Parser {
public:
    Parser(Lexer &lexer);
    double parse();

private:
    Lexer &l;
    std::optional<Tok::Token> currentTok;
    std::map<std::wstring, double> symtable;

    double parseV();
    double parseF();
    double parseTprime(double op1val);
    double parseT();
    double parseEprime(double op1val);
    double parseE();

    void nextTok();
    template<typename T>
    auto curTokVal() { return Tok::getValue<T>(currentTok.value()); }
};


#endif // PARSER_H
