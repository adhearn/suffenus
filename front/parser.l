%option nodefault noyywrap nounput noinput

%{
#include "lang.h"
#include <glib.h>
#include "parser.tab.h"
%}

%%
"=" { return yytext[0]; }
"," { return yytext[0]; }
";" { return yytext[0]; }
"(" { return yytext[0]; }
")" { return yytext[0]; }
"{" { return yytext[0]; }
"}" { return yytext[0]; }
"return" { return RETURN; }
"if" { return IF; }
"else" { return ELSE; }

"+" { yylval.op = Op_add; return BINOP; }
"-" { yylval.op = Op_sub; return BINOP; }
"*" { yylval.op = Op_mul; return BINOP; }
"/" { yylval.op = Op_div; return BINOP; }

"==" { yylval.op = Op_eq; return RELOP; }
"!=" { yylval.op = Op_neq; return RELOP; }
"<" { yylval.op = Op_lt; return RELOP; }
"<=" { yylval.op = Op_lte; return RELOP; }
">" { yylval.op = Op_gt; return RELOP; }
">=" { yylval.op = Op_gte; return RELOP; }

[0-9]+ { yylval.integer = atoi(yytext); return INTEGER;}
[[:alpha:]]([[:alnum:]])* { yylval.str = strdup(yytext);  return ID; }
[\n]+ { /* return NEWLINE; */ }
[ \t]+ { /* ignore whitespace */ }
. { printf("bad input character: '%s' at line %d\n", yytext, yylineno); }
%%
