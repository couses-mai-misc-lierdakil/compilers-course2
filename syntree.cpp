#include "syntree.h"

bool operator==(const NodeFunCall& a, const NodeFunCall& b){
 return a.name == b.name && a.args == b.args;
}

bool operator==(const NodeExp& a, const NodeExp& b) {
  return a.opType == b.opType
    && a.op1 == b.op1
    && a.op2 == b.op2;
}

bool operator==(const NodeUn& a, const NodeUn& b) {
  return a.opType == b.opType
    && a.op1 == b.op1;
}

bool NodeVal::operator==(const NodeVal &other) const {
    return value == other.value;
}

bool NodeVar::operator==(const NodeVar &other) const {
    return name == other.name;
}


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

std::size_t computeNodeHash(const Node &x)
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
