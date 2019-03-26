#ifndef DRIVER_H
#define DRIVER_H

#include "lex.yy.h"
#include <type_traits>

class Driver : public yy::Lexer {
public:
  using Lexer::Lexer;
  using symt_t = std::map<std::wstring, double>;
  using funt_t = std::map<std::wstring, Function>;
  template<typename ... Args>
  Driver(symt_t &symtable, funt_t& funtable, SynTree& syntree, Args&& ... args):
    Lexer(std::forward<Args>(args)...),
    symtable(symtable),
    funtable(funtable),
    syntree(syntree)
    {}
  const Node* createValNode(double val);
  const Node* createVarNode(std::wstring name);
  const Node* createExpNode(NodeExp::OpType opType, const Node* op1, const Node* op2);
  const Node* createUnNode(NodeUn::OpType opType, const Node* op1);
  const Node* createFunCallNode(std::wstring name, std::list<const Node*> args);
  double compute(const Node* x);
  std::list<SynTree::node_type> cleanSynTree(const Node* x);
  symt_t& symtable;
  funt_t& funtable;
  SynTree& syntree;
  double result;
};

#endif
