#ifndef DRIVER_H
#define DRIVER_H

#include "lex.yy.h"

class Driver : public yy::Lexer {
public:
  using Lexer::Lexer;
  std::map<std::wstring, double> symtable;
  double result;
};

#endif
