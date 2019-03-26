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
