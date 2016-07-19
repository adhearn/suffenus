%option nodefault noyywrap

%{
#include "lang.h"
#include "list.h"
#include "parser.tab.h"
#define TRUE 1
#define FALSE 0
%}

%%
"+" |
"-" |
"*" |
"/" |
"=" |
"," |
";" |
"(" |
")" { return yytext[0]; }

"if" { return IF; }
"else" { return ELSE; }
"return" { return RETURN; }
"==" { yylval.op = Op_eq; return EQUALITY; }
"!=" { yylval.op = Op_neq; return EQUALITY; }
"<" { yylval.op = Op_lt; return CMP; }
"<=" { yylval.op = Op_lte; return CMP; }
">" { yylval.op = Op_gt; return CMP; }
">=" { yylval.op = Op_gte; return CMP; }
"<<" { yylval.op = Op_lshift; return SHIFT; }
">>" { yylval.op = Op_rshift; return SHIFT; }
"&&" { return AND; }
"||" { return OR; }
[0-9]+ { yylval.integer = atoi(yytext); return INTEGER;}
[ \t\n]+ { /* ignore whitespace */ }
[A-Za-z]([A-Za-z]|[0-9])* { yylval.str = strdup(yytext); return ID; }
%%
