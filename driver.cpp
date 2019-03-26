#include "driver.h"
#include "utils.h"

const Node* Driver::createValNode(double val) {
    auto node = syntree.emplace(NodeVal{val});
    return &*node.first;
}

const Node* Driver::createVarNode(std::wstring name) {
    auto node = syntree.emplace(NodeVar{name});
    return &*node.first;
}

const Node* Driver::createExpNode(NodeExp::OpType opType, const Node *op1, const Node *op2) {
    auto node = syntree.emplace(NodeExp{opType, op1, op2});
    return &*node.first;
}

const Node* Driver::createUnNode(NodeUn::OpType opType, const Node *op1) {
    auto node = syntree.emplace(NodeUn{opType, op1});
    return &*node.first;
}

const Node* Driver::createFunCallNode(std::wstring name, std::list<const Node *> args) {
    auto node = syntree.emplace(NodeFunCall{name, args});
    return &*node.first;
}

double Driver::compute(const Node *x) {
  return std::visit([this](auto&& arg) -> double {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<NodeExp, T>) {
        switch(arg.opType) {
        case NodeExp::OpType::Add:
            return compute(arg.op1) + compute(arg.op2);
        case NodeExp::OpType::Sub:
            return compute(arg.op1) - compute(arg.op2);
        case NodeExp::OpType::Mul:
            return compute(arg.op1) * compute(arg.op2);
        case NodeExp::OpType::Div:
            return compute(arg.op1) / compute(arg.op2);
        default:
            throw new std::runtime_error("Unknown Exp::OpType");
        }
    } else if constexpr (std::is_same_v<NodeUn, T>) {
        if(arg.opType == NodeUn::OpType::Neg)
            return -compute(arg.op1);
        else
            throw new std::runtime_error("Unknown Un::OpType");
    } else if constexpr (std::is_same_v<NodeVal, T>) {
        return arg.value;
    } else if constexpr (std::is_same_v<NodeVar, T>) {
        return symtable.at(arg.name);
    } else if constexpr (std::is_same_v<NodeFunCall, T>) {
        symt_t tempst(symtable);
        symt_t &oldsymt = symtable;
        auto fun = funtable.at(arg.name);
        auto argit = fun.args.begin();
        auto argvalit = arg.args.begin();
        for (; argit != fun.args.end() && argvalit != arg.args.end(); ++argit, ++argvalit) {
            tempst[*argit] = compute(*argvalit);
        }
        symtable = tempst;
        auto result = compute(fun.body);
        symtable = oldsymt;
        return result;
    } else {
        static_assert(always_false<T>, "Unknown node type in compute");
    }
  }, *x);
}

std::list<SynTree::node_type> Driver::cleanSynTree(const Node *x) {
    std::list<SynTree::node_type> l;
    std::visit([&l, this](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<NodeExp,T>) {
          l.splice(l.end(), cleanSynTree(arg.op1));
          l.splice(l.end(), cleanSynTree(arg.op2));
      } else if constexpr (std::is_same_v<NodeUn, T>) {
          l.splice(l.end(), cleanSynTree(arg.op1));
      } else if constexpr (std::is_same_v<NodeFunCall, T>) {
          for (auto &i: arg.args) {
              l.splice(l.end(), cleanSynTree(i));
          }
      }
    }, *x);
    l.push_back(syntree.extract(*x));
    return l;
}
