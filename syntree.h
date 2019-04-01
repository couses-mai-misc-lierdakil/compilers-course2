#ifndef SYNTREE_H
#define SYNTREE_H

#include <list>
#include <string>
#include <unordered_set>
#include <variant>

struct NodeVal {
  double value;
  bool operator==(const NodeVal &other) const;
};

struct NodeVar {
  std::wstring name;
  bool operator==(const NodeVar &other) const;
};

struct NodeFunCall;
struct NodeExp;
struct NodeUn;
struct NodeCond;

using Node =
    std::variant<NodeExp, NodeUn, NodeVal, NodeVar, NodeFunCall, NodeCond>;

struct NodeFunCall {
  std::wstring name;
  std::list<const Node *> args;
};

bool operator==(const NodeFunCall &a, const NodeFunCall &b);

struct NodeExp {
  enum class OpType { Add, Sub, Mul, Div };
  OpType opType;
  const Node *op1;
  const Node *op2;
};

struct NodeUn {
  enum class OpType { Neg };
  OpType opType;
  const Node *op1;
};

struct NodeCond {
  const Node *cond;
  const Node *ifTrue;
  const Node *ifFalse;
};

bool operator==(const NodeExp &a, const NodeExp &b);
bool operator==(const NodeUn &a, const NodeUn &b);
bool operator==(const NodeCond &a, const NodeCond &b);

using SynTree = std::unordered_set<Node>;

struct Function {
  using arglist_t = std::list<std::wstring>;
  arglist_t args;
  const Node *body;
};

std::size_t computeNodeHash(const Node &x);

namespace std {
template <> struct hash<Node> {
  size_t operator()(const Node &x) const { return computeNodeHash(x); }
};
} // namespace std

#endif
