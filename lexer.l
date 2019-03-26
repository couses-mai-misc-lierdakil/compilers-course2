%option bison-cc bison-locations unicode
%option namespace=yy

%top{
  #include "parser.tab.hh"
%}

ident   [\p{Ll}_\p{Lu}\p{Lt}][\p{Ll}_\p{Lu}\p{Lt}\p{Nd}']*
num     [0-9]+([.][0-9]*)?([eE][+-]?[0-9]+)?

%%

def      return parser::token::Def;
MEMSTAT  return parser::token::Memstat;
{num}    yylval.emplace<std::wstring>(wstr()); return parser::token::Num;
{ident}  yylval.emplace<std::wstring>(wstr()); return parser::token::Var;
[()+*/=\-] return chr();
[\p{Zs}] // ignore spaces
.        return chr();

%%
