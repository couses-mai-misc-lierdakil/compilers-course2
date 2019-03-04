#ifndef LEXER_H
#define LEXER_H

#include <list>
#include <istream>
#include <memory>

class Token;

class Lexer
{
public:
    Lexer(std::wistream& inputStream);
    std::unique_ptr<Token> getNextToken();
private:
    std::list<wchar_t> ret_buf;
    std::wistream& inputStream;
    wint_t getNextChar();
};

#endif // LEXER_H
