#include "driver.h"

const Node &Driver::createValNode(double val) {
    auto node = syntree.emplace(NodeVal{val});
    return *node.first;
}

const Node &Driver::createVarNode(std::wstring name) {
    auto node = syntree.emplace(NodeVar{name});
    return *node.first;
}

const Node &Driver::createExpNode(NodeExp::OpType opType, const Node &op1, const Node &op2) {
    auto node = syntree.emplace(NodeExp{opType, op1, op2});
    return *node.first;
}

const Node &Driver::createUnNode(NodeUn::OpType opType, const Node &op1) {
    auto node = syntree.emplace(NodeUn{opType, op1});
    return *node.first;
}

const Node &Driver::createFunCallNode(std::wstring name, std::list<const Node *> args) {
    auto node = syntree.emplace(NodeFunCall{name, args});
    return *node.first;
}

double Driver::compute(const Node &x) {
    if (auto exp = std::get_if<NodeExp>(&x)) {
        switch(exp->opType) {
        case NodeExp::OpType::Add:
            return compute(exp->op1) + compute(exp->op2);
        case NodeExp::OpType::Sub:
            return compute(exp->op1) - compute(exp->op2);
        case NodeExp::OpType::Mul:
            return compute(exp->op1) * compute(exp->op2);
        case NodeExp::OpType::Div:
            return compute(exp->op1) / compute(exp->op2);
        default:
            throw new std::runtime_error("Unknown Exp::OpType");
        }
    } else if (auto exp = std::get_if<NodeUn>(&x)) {
        if(exp->opType == NodeUn::OpType::Neg)
            return -compute(exp->op1);
        else
            throw new std::runtime_error("Unknown Un::OpType");
    } else if (auto val = std::get_if<NodeVal>(&x)) {
        return val->value;
    } else if (auto var = std::get_if<NodeVar>(&x)) {
        return symtable.at(var->name);
    } else if (auto funcall = std::get_if<NodeFunCall>(&x)) {
        symt_t tempst(symtable);
        symt_t &oldsymt = symtable;
        auto fun = funtable.at(funcall->name);
        auto argit = fun.args.begin();
        auto argvalit = funcall->args.begin();
        for (; argit != fun.args.end() && argvalit != funcall->args.end(); ++argit, ++argvalit) {
            tempst[*argit] = compute(**argvalit);
        }
        symtable = tempst;
        auto result = compute(fun.body);
        symtable = oldsymt;
        return result;
    } else {
        throw std::runtime_error("Unknown node type in compute");
    }
}

std::list<SynTree::node_type> Driver::cleanSynTree(const Node &x) {
    std::list<SynTree::node_type> l;
    if (auto exp = std::get_if<NodeExp>(&x)) {
        l.splice(l.end(), cleanSynTree(exp->op1));
        l.splice(l.end(), cleanSynTree(exp->op2));
    } else if (auto exp = std::get_if<NodeUn>(&x)) {
        l.splice(l.end(), cleanSynTree(exp->op1));
    } else if (auto funcall = std::get_if<NodeFunCall>(&x)) {
        for (auto &i: funcall->args) {
            l.splice(l.end(), cleanSynTree(*i));
        }
    }
    l.push_back(syntree.extract(x));
    return l;
}
