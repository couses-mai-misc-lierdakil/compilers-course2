#include "syntree.h"
#include "utils.h"

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

template <typename _Tp, typename _Up>
  inline constexpr bool is_t = std::is_same<_Tp, _Up>::value;

std::size_t computeNodeHash(const Node &x)
{
  return std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (is_t<NodeExp, T>) {
      return hash_combine_calc(arg.opType, arg.op1, arg.op2);
    } else if constexpr (is_t<NodeVal, T>) {
      return hash_combine_calc(arg.value);
    } else if constexpr (is_t<NodeVar, T>) {
      return hash_combine_calc(arg.name);
    } else if constexpr (is_t<NodeUn, T>) {
      return hash_combine_calc(arg.opType, arg.op1);
    } else if constexpr (is_t<NodeFunCall, T>) {
      auto h = hash_combine_calc(arg.name);
      for (auto const &i : arg.args) {
        h = hash_combine(h, myhash(i));
      }
      return h;
    } else {
      static_assert(always_false<T>, "Unknown node type in hash");
    }
  }, x);
}
