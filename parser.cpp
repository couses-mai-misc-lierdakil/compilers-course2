#include <cmath>
#include <sstream>
#include <locale>
#include <codecvt>
#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "utils.h"

/* Grammar:
 * E -> T E'
 * E' -> + T E' | - T E' | eps
 * T -> F T'
 * T' -> * F T' | / F T' | eps
 * F -> V | - V
 * V -> num | var
 */

Parser::Parser(Lexer &lexer) : l(lexer) {
    symtable[L"pi"] = M_PI;
}

double Parser::parse()
{
    nextTok();
    return parseE();
}

double Parser::parseV() {
    double val;
    if (auto tv = curTokVal<TokenType::Num>()) {
        val = tv.value();
    } else if (auto tv = curTokVal<TokenType::Var>()) {
        val = symtable.at(tv.value());
    } else {
        throw runtime_error(
                    L"error in V: expected Num or Var, got "
                    +currentTok->toString());
    }
    nextTok();
    return val;
}

double Parser::parseF() {
    if (curTokVal<TokenType::Op>() == '-') {
        nextTok();
        return -parseV();
    } else {
        return parseV();
    }
}

double Parser::parseTprime(double op1val) {
    if(curTokVal<TokenType::Op>() == '*') {
        nextTok();
        return parseTprime(op1val*parseF());
    } else if(curTokVal<TokenType::Op>() == '/') {
        nextTok();
        return parseTprime(op1val/parseF());
    } else {
        return op1val;
    }
}

double Parser::parseT() {
    double op1val = parseF();
    return parseTprime(op1val);
}

double Parser::parseEprime(double op1val) {
    if(curTokVal<TokenType::Op>() == '+') {
        nextTok();
        return parseEprime(op1val + parseT());
    } else if(curTokVal<TokenType::Op>() == '-') {
        nextTok();
        return parseEprime(op1val-parseT());
    } else {
        return op1val;
    }
}

double Parser::parseE() {
    double op1val = parseT();
    return parseEprime(op1val);
}

void Parser::nextTok()
{
    currentTok = l.getNextToken();
}
