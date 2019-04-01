#include "driver.h"
#include "utils.h"
#include <cmath>
#include <optional>
#include <queue>

const Node *Driver::createValNode(double val) {
  auto node = st.syntree.emplace(NodeVal{val});
  return &*node.first;
}

const Node *Driver::createVarNode(std::wstring name) {
  auto node = st.syntree.emplace(NodeVar{name});
  return &*node.first;
}

const Node *Driver::createExpNode(NodeExp::OpType opType, const Node *op1,
                                  const Node *op2) {
  auto node = st.syntree.emplace(NodeExp{opType, op1, op2});
  return &*node.first;
}

const Node *Driver::createUnNode(NodeUn::OpType opType, const Node *op1) {
  auto node = st.syntree.emplace(NodeUn{opType, op1});
  return &*node.first;
}

const Node *Driver::createFunCallNode(std::wstring name,
                                      std::list<const Node *> args) {
  auto node = st.syntree.emplace(NodeFunCall{name, args});
  return &*node.first;
}

const Node *Driver::createCondNode(const Node *cond, const Node *ifTrue,
                                   const Node *ifFalse) {
  auto node = st.syntree.emplace(NodeCond{cond, ifTrue, ifFalse});
  return &*node.first;
}

CNodeVal operator+(const CNodeVal &a, const CNodeVal &b) {
  if (a.valType == NodeValType::Num && a.valType == NodeValType::Num) {
    return CNodeVal{a.value + b.value, NodeValType::Num};
  }
  throw std::runtime_error("Type error");
}
CNodeVal operator-(const CNodeVal &a, const CNodeVal &b) {
  if (a.valType == NodeValType::Num && a.valType == NodeValType::Num) {
    return CNodeVal{a.value - b.value, NodeValType::Num};
  }
  throw std::runtime_error("Type error");
}
CNodeVal operator*(const CNodeVal &a, const CNodeVal &b) {
  if (a.valType == NodeValType::Num && a.valType == NodeValType::Num) {
    return CNodeVal{a.value * b.value, NodeValType::Num};
  }
  throw std::runtime_error("Type error");
}
CNodeVal operator/(const CNodeVal &a, const CNodeVal &b) {
  if (a.valType == NodeValType::Num && a.valType == NodeValType::Num) {
    return CNodeVal{a.value / b.value, NodeValType::Num};
  }
  throw std::runtime_error("Type error");
}
CNodeVal operator-(const CNodeVal &a) {
  if (a.valType == NodeValType::Num) {
    return CNodeVal{-a.value, NodeValType::Num};
  }
  throw std::runtime_error("Type error");
}
CNodeVal operator>(const CNodeVal &a, const CNodeVal &b) {
  if (a.valType == NodeValType::Num && a.valType == NodeValType::Num) {
    return CNodeVal{a.value > b.value ? 1.0 : 0.0, NodeValType::Bool};
  }
  throw std::runtime_error("Type error");
}
CNodeVal operator<(const CNodeVal &a, const CNodeVal &b) {
  if (a.valType == NodeValType::Num && a.valType == NodeValType::Num) {
    return CNodeVal{a.value < b.value ? 1.0 : 0.0, NodeValType::Bool};
  }
  throw std::runtime_error("Type error");
}
CNodeVal operator>=(const CNodeVal &a, const CNodeVal &b) {
  if (a.valType == NodeValType::Num && a.valType == NodeValType::Num) {
    return CNodeVal{a.value >= b.value ? 1.0 : 0.0, NodeValType::Bool};
  }
  throw std::runtime_error("Type error");
}
CNodeVal operator<=(const CNodeVal &a, const CNodeVal &b) {
  if (a.valType == NodeValType::Num && a.valType == NodeValType::Num) {
    return CNodeVal{a.value <= b.value ? 1.0 : 0.0, NodeValType::Bool};
  }
  throw std::runtime_error("Type error");
}
CNodeVal operator==(const CNodeVal &a, const CNodeVal &b) {
  if (a.valType == NodeValType::Num && a.valType == NodeValType::Num) {
    return CNodeVal{a.value == b.value ? 1.0 : 0.0, NodeValType::Bool};
  } else if (a.valType == NodeValType::Bool && a.valType == NodeValType::Bool) {
    return CNodeVal{a.value == b.value ? 1.0 : 0.0, NodeValType::Bool};
  }
  throw std::runtime_error("Type error");
}
CNodeVal operator!=(const CNodeVal &a, const CNodeVal &b) {
  if (a.valType == NodeValType::Num && a.valType == NodeValType::Num) {
    return CNodeVal{a.value != b.value ? 1.0 : 0.0, NodeValType::Bool};
  } else if (a.valType == NodeValType::Bool && a.valType == NodeValType::Bool) {
    return CNodeVal{a.value != b.value ? 1.0 : 0.0, NodeValType::Bool};
  }
  throw std::runtime_error("Type error");
}

CNodeVal Driver::compute(const Node *x,
                         std::optional<const DriverState::symt_t *> symtable) {
  if (!symtable)
    symtable = &st.symtable;
  return std::visit(
      [this, &symtable](auto &&arg) -> CNodeVal {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<NodeExp, T>) {
          switch (arg.opType) {
          case NodeExp::OpType::Add:
            return compute(arg.op1, symtable) + compute(arg.op2, symtable);
          case NodeExp::OpType::Sub:
            return compute(arg.op1, symtable) - compute(arg.op2, symtable);
          case NodeExp::OpType::Mul:
            return compute(arg.op1, symtable) * compute(arg.op2, symtable);
          case NodeExp::OpType::Div:
            return compute(arg.op1, symtable) / compute(arg.op2, symtable);
          case NodeExp::OpType::Lte:
            return compute(arg.op1, symtable) <= compute(arg.op2, symtable);
          case NodeExp::OpType::Gte:
            return compute(arg.op1, symtable) >= compute(arg.op2, symtable);
          case NodeExp::OpType::Lt:
            return compute(arg.op1, symtable) < compute(arg.op2, symtable);
          case NodeExp::OpType::Gt:
            return compute(arg.op1, symtable) > compute(arg.op2, symtable);
          case NodeExp::OpType::Eq:
            return compute(arg.op1, symtable) == compute(arg.op2, symtable);
          case NodeExp::OpType::Neq:
            return compute(arg.op1, symtable) != compute(arg.op2, symtable);
          default:
            throw new std::runtime_error("Unknown Exp::OpType");
          }
        } else if constexpr (std::is_same_v<NodeUn, T>) {
          if (arg.opType == NodeUn::OpType::Neg)
            return -compute(arg.op1, symtable);
          else if (arg.opType == NodeUn::OpType::BNeg) {
            auto val = compute(arg.op1, symtable);
            if (val.valType != NodeValType::Bool)
              throw std::runtime_error("Type error");
            return CNodeVal{compute(arg.op1, symtable).value == 1.0 ? 0.0 : 1.0,
                            NodeValType::Bool};
          } else
            throw new std::runtime_error("Unknown Un::OpType");
        } else if constexpr (std::is_same_v<NodeVal, T>) {
          return CNodeVal{arg.value, NodeValType::Num};
        } else if constexpr (std::is_same_v<NodeVar, T>) {
          return symtable.value()->at(arg.name);
        } else if constexpr (std::is_same_v<NodeCond, T>) {
          auto cond = compute(arg.cond, symtable);
          if (cond.valType != NodeValType::Bool)
            throw std::runtime_error("Type error");
          if (cond.value == 0) {
            return compute(arg.ifFalse, symtable);
          } else {
            return compute(arg.ifTrue, symtable);
          }
        } else if constexpr (std::is_same_v<NodeFunCall, T>) {
          auto funit = st.funtable.find(arg.name);
          if (funit == st.funtable.end()) {
            if (arg.name == L"sqrt") {
              if (arg.args.size() != 1) {
                throw new std::runtime_error("Argument number mismatch");
              }
              auto val = compute(arg.args.front(), symtable);
              if (val.valType != NodeValType::Num)
                throw std::runtime_error("Type error");
              return CNodeVal{std::sqrt(val.value), NodeValType::Num};
            } else if (arg.name == L"sin") {
              if (arg.args.size() != 1) {
                throw new std::runtime_error("Argument number mismatch");
              }
              auto val = compute(arg.args.front(), symtable);
              if (val.valType != NodeValType::Num)
                throw std::runtime_error("Type error");
              return CNodeVal{std::sin(val.value), NodeValType::Num};
            } else if (arg.name == L"cos") {
              if (arg.args.size() != 1) {
                throw new std::runtime_error("Argument number mismatch");
              }
              auto val = compute(arg.args.front(), symtable);
              if (val.valType != NodeValType::Num)
                throw std::runtime_error("Type error");
              return CNodeVal{std::cos(val.value), NodeValType::Num};
            } else {
              throw new std::runtime_error("Undefined function");
            }
          }
          auto fun = funit->second;
          if (fun.args.size() != arg.args.size()) {
            throw new std::runtime_error("Argument number mismatch");
          }
          auto argit = fun.args.begin();
          auto argvalit = arg.args.begin();
          DriverState::symt_t tempst;
          for (; argit != fun.args.end() && argvalit != arg.args.end();
               ++argit, ++argvalit) {
            tempst[*argit] = compute(*argvalit, symtable);
          }
          return compute(fun.body, &tempst);
        } else {
          static_assert(always_false<T>, "Unknown node type in compute");
        }
      },
      *x);
}

struct RecTreeVis {
  std::unordered_set<const SynTree::value_type *> &roots;
  template <typename U> void operator()(U &&arg) {
    auto rec = RecTreeVis{roots};
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<NodeExp, T>) {
      roots.insert(arg.op1);
      std::visit(rec, *arg.op1);
      roots.insert(arg.op2);
      std::visit(rec, *arg.op2);
    } else if constexpr (std::is_same_v<NodeUn, T>) {
      roots.insert(arg.op1);
      std::visit(rec, *arg.op1);
    } else if constexpr (std::is_same_v<NodeFunCall, T>) {
      for (auto &i : arg.args) {
        roots.insert(i);
        std::visit(rec, *i);
      }
    } else if constexpr (std::is_same_v<NodeCond, T>) {
      roots.insert(arg.cond);
      std::visit(rec, *arg.cond);
      roots.insert(arg.ifTrue);
      std::visit(rec, *arg.ifTrue);
      roots.insert(arg.ifFalse);
      std::visit(rec, *arg.ifFalse);
    }
  }
};

void Driver::cleanSynTree(const Node *x) {
  std::unordered_set<const SynTree::value_type *> roots;
  for (auto &i : st.funtable) {
    roots.insert(i.second.body);
    std::visit(RecTreeVis{roots}, *i.second.body);
  }

  std::queue<const SynTree::value_type *> itemsToErase;
  for (auto &i : st.syntree) {
    if (roots.find(&i) == roots.end()) {
      itemsToErase.push(&i);
    }
  }
  while (!itemsToErase.empty()) {
    st.syntree.erase(*itemsToErase.front());
    itemsToErase.pop();
  }
}

void Driver::printMemStat() {
  std::wcerr << "Number of variables: " << st.symtable.size() << "\n"
             << "Number of functions: " << st.funtable.size() << "\n"
             << "Syntax nodes stored: " << st.syntree.size() << std::endl;
}
