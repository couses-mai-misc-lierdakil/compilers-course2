#ifndef DRIVER_H
#define DRIVER_H

#include "lex.yy.h"
#include <type_traits>

class Driver : public yy::Lexer {
public:
  using Lexer::Lexer;
  using symt_t = std::map<std::wstring, double>;
  template<typename ... Args>
  Driver(symt_t &symtable, Args&& ... args):
    Lexer(std::forward<Args>(args)...), symtable(symtable) {}
  symt_t& symtable;
  double result;
};

#endif
