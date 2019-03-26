#ifndef SYNTREE_H
#define SYNTREE_H

#include <unordered_set>
#include <string>
#include <variant>
#include <list>

struct NodeVal {
  double value;
  bool operator==(const NodeVal& other) const;
};

struct NodeVar {
  std::wstring name;
  bool operator==(const NodeVar& other) const;
};

struct NodeFunCall;
struct NodeExp;
struct NodeUn;

using Node = std::variant<NodeExp, NodeUn, NodeVal, NodeVar, NodeFunCall>;

struct NodeFunCall {
  std::wstring name;
  std::list<const Node*> args;
};

bool operator==(const NodeFunCall& a, const NodeFunCall& b);

struct NodeExp {
  enum class OpType {
    Add, Sub, Mul, Div
  };
  OpType opType;
  const Node* op1;
  const Node* op2;
};

struct NodeUn {
  enum class OpType {
    Neg
  };
  OpType opType;
  const Node* op1;
};

bool operator==(const NodeExp& a, const NodeExp& b);
bool operator==(const NodeUn& a, const NodeUn& b);

using SynTree = std::unordered_set<Node>;

struct Function {
  using arglist_t = std::list<std::wstring>;
  arglist_t args;
  const Node* body;
};

std::size_t computeNodeHash(const Node &x);

namespace std {
  template <> struct hash<Node>
  {
    size_t operator()(const Node & x) const {
        return computeNodeHash(x);
    }
  };
}

#endif
