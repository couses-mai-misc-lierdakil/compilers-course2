%require  "3.2"
%language "c++"
%define api.value.type variant
%code requires {
  class Driver;
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
%printer { yyo << $$; } <*>
%define parse.trace
%define parse.error verbose
%parse-param { Driver& drv }

%type <double> exp
%type <double> val
%token <std::wstring> Num "number"
%token <std::wstring> Var "identifier"
%token '(' ')' '+' '-' '*' '/' '=' ';'

%left ';'
%right '='
%left '+' '-'
%left '*' '/'
%precedence UNEG
%%
%start s;
s: exp                  { drv.result = $1; }

exp:
  exp '+' exp           { $$ = $1 + $3; }
| exp '-' exp           { $$ = $1 - $3; }
| exp '*' exp           { $$ = $1 * $3; }
| exp '/' exp           { $$ = $1 / $3; }
| '(' exp ')'           { $$ = $2; }
| '-' exp %prec UNEG    { $$ = -$2; }
| val                   { $$ = $1; }
| Var '=' exp           { drv.symtable[$1] = $3; $$ = $3; }
;

val:
  Var { $$ = drv.symtable[$1]; }
| Num { $$ = std::stod($1); }
;

%%
void yy::parser::error(const location_type& loc, const std::string& msg)
{
  std::cerr << msg << " at " << loc << std::endl;
}
