#ifndef SYNTREE_H
#define SYNTREE_H

#include <unordered_set>
#include <string>
#include <variant>
#include <list>

struct NodeVal {
  double value;
  bool operator==(const NodeVal& other) const {
    return value == other.value;
  }
};

struct NodeVar {
  std::wstring name;
  bool operator==(const NodeVar& other) const {
    return name == other.name;
  }
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
  const Node& op1;
  const Node& op2;
};

struct NodeUn {
  enum class OpType {
    Neg
  };
  OpType opType;
  const Node& op1;
};

bool operator==(const NodeExp& a, const NodeExp& b);
bool operator==(const NodeUn& a, const NodeUn& b);

using SynTree = std::unordered_set<Node>;

struct Function {
  using arglist_t = std::list<std::wstring>;
  arglist_t args;
  const Node& body;
};

template <typename ... T>
inline std::size_t hash_combine(const std::size_t & s, const T& ... args)
{
  return s ^ (hash_combine(args...) + 0x9e3779b9 + (s<< 6) + (s>> 2));
}

template <>
inline std::size_t hash_combine<>(const std::size_t & s)
{
  return s;
}

template <typename T>
std::size_t myhash(const T& v) {
  std::hash<T> h;
  return h(v);
}

template<typename ... Ts>
std::size_t hash_combine_calc(const Ts& ... args) {
  return hash_combine(myhash(args)...);
}

namespace std {
  template <> struct hash<Node>
  {
    size_t operator()(const Node & x) const
    {
        if (auto exp = std::get_if<NodeExp>(&x)) {
          return hash_combine_calc(exp->opType, exp->op1, exp->op2);
        } else if (auto val = std::get_if<NodeVal>(&x)) {
          return hash_combine_calc(val->value);
        } else if (auto var = std::get_if<NodeVar>(&x)) {
          return hash_combine_calc(var->name);
        } else if (auto var = std::get_if<NodeUn>(&x)) {
          return hash_combine_calc(var->opType, var->op1);
        } else if (auto funcall = std::get_if<NodeFunCall>(&x)) {
          auto h = hash_combine_calc(funcall->name);
          for (auto const &i : funcall->args) {
            h = hash_combine(h, myhash(i));
          }
          return h;
        } else {
          throw std::runtime_error("Unknown node type in hash");
        }
    }
  };
}

#endif
