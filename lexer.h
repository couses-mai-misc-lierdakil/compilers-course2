#ifndef LEXER_H
#define LEXER_H

#include <istream>
#include <list>
#include "token.h"

class Lexer
{
public:
    Lexer(std::wistream& inputStream);
    Tok::Token getNextToken();
private:
    std::list<wchar_t> ret_buf;
    std::wistream& inputStream;
    wint_t getNextChar();
};

#endif // LEXER_H
