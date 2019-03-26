#ifndef DRIVER_H
#define DRIVER_H

#include "lex.yy.h"
#include <type_traits>

struct DriverState {
  using symt_t = std::map<std::wstring, double>;
  using funt_t = std::map<std::wstring, Function>;
  symt_t symtable;
  funt_t funtable;
  SynTree syntree;
};

class Driver : public yy::Lexer {
public:
  using Lexer::Lexer;
  template <typename... Args>
  Driver(DriverState &st, Args &&... args)
      : Lexer(std::forward<Args>(args)...), st(st) {}
  const Node *createValNode(double val);
  const Node *createVarNode(std::wstring name);
  const Node *createExpNode(NodeExp::OpType opType, const Node *op1,
                            const Node *op2);
  const Node *createUnNode(NodeUn::OpType opType, const Node *op1);
  const Node *createFunCallNode(std::wstring name,
                                std::list<const Node *> args);
  double
  compute(const Node *x,
          std::optional<const DriverState::symt_t *> symtable = std::nullopt);
  void cleanSynTree(const Node *x);
  void printMemStat();
  DriverState &st;
  std::optional<double> result = std::nullopt;
};

#endif
