%require  "3.2"
%language "c++"
%define api.value.type variant
%code requires {
  class Driver;
  #include "syntree.h"
}
%code{
  #include <string>
  #include "driver.h"
  #include "utils.h"
  #undef yylex
  #define yylex drv.lex
}
%locations
%printer { yyo << wstr_to_str($$); } <std::wstring>
%printer { yyo << ""; } <*>
%define parse.trace
%define parse.error verbose
%parse-param { Driver& drv }

%type <const Node*> exp callarg val
%type <Function::arglist_t> args
%type <std::list<const Node*>> callargs
%token <std::wstring> Num "number"
%token <std::wstring> Var "identifier"
%token Def "def"
%token If "if"
%token Then "then"
%token Else "else"
%token Memstat "MEMSTAT"
%token Lte "<="
%token Gte ">="
%token Eq "=="
%token Neq "!="
%token '(' ')' '+' '-' '*' '/' '=' '<' '>' '~'

%right If Then Else
%right '='
%left '<' '>' Lte Gte Eq Neq
%precedence '~' BNEG
%left '+' '-'
%left '*' '/'
%precedence UNEG
%%
%start s;
s: exp                  { drv.result = drv.compute($1); drv.cleanSynTree($1); }
|  stmt

stmt:
  Def Var '(' args ')' '=' exp
                        { drv.st.funtable.emplace($2, Function{$4, $7}); }
| Var '=' exp           { drv.st.symtable[$1] = drv.compute($3); }
| Memstat               { drv.printMemStat(); }

args:
  Var                   { $$ = Function::arglist_t({$1}); }
| Var ',' args          { $3.emplace_front($1); $$ = $3; }
;

exp:
  exp '+' exp           { $$ = drv.createExpNode(NodeExp::OpType::Add, $1, $3); }
| exp '-' exp           { $$ = drv.createExpNode(NodeExp::OpType::Sub, $1, $3); }
| exp '*' exp           { $$ = drv.createExpNode(NodeExp::OpType::Mul, $1, $3); }
| exp '/' exp           { $$ = drv.createExpNode(NodeExp::OpType::Div, $1, $3); }
| exp '<' exp           { $$ = drv.createExpNode(NodeExp::OpType::Lt, $1, $3); }
| exp '>' exp           { $$ = drv.createExpNode(NodeExp::OpType::Gt, $1, $3); }
| exp Lte exp           { $$ = drv.createExpNode(NodeExp::OpType::Lte, $1, $3); }
| exp Gte exp           { $$ = drv.createExpNode(NodeExp::OpType::Gte, $1, $3); }
| exp Eq exp            { $$ = drv.createExpNode(NodeExp::OpType::Eq, $1, $3); }
| exp Neq exp           { $$ = drv.createExpNode(NodeExp::OpType::Neq, $1, $3); }
| '(' exp ')'           { $$ = $2; }
| '-' exp %prec UNEG    { $$ = drv.createUnNode(NodeUn::OpType::Neg, $2); }
| '~' exp %prec BNEG    { $$ = drv.createUnNode(NodeUn::OpType::BNeg, $2); }
| val                   { $$ = $1; }
| If exp Then exp Else exp { $$ = drv.createCondNode($2, $4, $6); }
;

val:
  Var { $$ = drv.createVarNode($1); }
| Num { $$ = drv.createValNode(std::stod($1)); }
| Var '(' callargs ')' { $$ = drv.createFunCallNode($1, $3); }
;

callargs:
  callarg               { $$ = std::list<const Node*>({$1}); }
| callarg ',' callargs      { $3.emplace_front($1); $$ = $3; }
;

callarg:
  exp { $$ = $1; }
;

%%
void yy::parser::error(const location_type& loc, const std::string& msg)
{
  std::cerr << msg << " at " << loc << std::endl;
}
