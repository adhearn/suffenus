%{

#include <stdio.h>
#include <glib.h>
#include "util.h"
#include "lang.h"

#define YYDEBUG 1

int yylex();
int yyerror(struct Program **prog, const char *msg);
extern int yylineno;
struct SymbolTable *symbol_table;

%}

%define parse.error verbose

%union {
    int integer;
    char *str;
    GList *list;
    enum Op op;
    struct Constant *constant;
    struct Identifier *identifier;
    struct Declaration *declaration;
    struct Expr *expr;
    struct Statement *statement;
    struct Program *program;
    struct Function *function;
    struct Type *type;
    struct Statement_assignment *assignment;
    struct AstNode *node;
}

%parse-param {struct Program **prog}

%start program
//                      %token IF
//                      %token RETURN
%token                  RELOP UNOP BINOP
%token                  ID
%token                  INTEGER
%token                  SEMI
%token                  NEWLINE
%token                  LPAREN
%token                  RPAREN
%token                  LBRACE
%token                  RBRACE
%token                  RETURN
%token                  COMMA
%token                  EQUALS


%type   <program>       program
%type   <integer>       INTEGER
%type   <op>            RELOP BINOP UNOP
%type   <list>          top_level_block block
%type   <node>          top_level_block_element block_element
%type   <statement>     statement
%type   <declaration>   declaration
%type   <type>          type
%type   <str>           ID
%type   <function>      function
%type   <expr>          expr
%type   <assignment>    assignment

%left EQUALS
%left RELOP
%left BINOP

%%

program:
                top_level_block {
                    struct Program *full_program = make_Program($1);
                    *prog = full_program;
                    symbol_type = symbol_table_new(NULL);
                }
                ;
top_level_block:
                top_level_block_element {
                    GList *list = NULL;
                    list = g_list_append(list, $1);
                    $$ = list;
                }
        |
                top_level_block top_level_block_element {
                    GList *list = g_list_append($1, $2);
                    $$ = list;
                }
                ;

top_level_block_element:
                declaration { $$ = $1; }
        |
                function { $$ = $1; }
                ;

declaration:
                type ID SEMI {
                    struct Identifier *identifier = make_Identifier($2, $1);
                    symbol_table_extend(symbol_table, identifier);
                    struct Declaration *decl = make_Declaration($1, identifier);
                    $$ = decl;
                }
                ;

block:
                block block_element {
                    GList *list = g_list_append($1, $2);
                    $$ = list;
                }
        |
                block_element {
                    GList *list = NULL;
                    list = g_list_append(list, $1);
                    $$ = list;
                }
        ;

block_element:
                declaration { $$ = $1; }
        |       statement { $$ = $1; }
        ;

function:
                /*
                * Once we have function params, we'll have to fix how we do symbol tables,
                * since we'll need to add the function params to the symbol table for the
                * function, not the block above. For now, though, we can wait to create the
                * new symbol table until we process this production.
                *
                * NB: the function name itself will still belong in the outer scope!
                */
                type ID LPAREN RPAREN LBRACE block RBRACE {
                    struct Identifier *identifier = make_Identifier($2);
                    Type_make_fn_type($1);
                    identifier->type = $1;
                    symbol_table_extend(symbol_table, identifier);
                    symbol_table = symbol_table_new(symbol_table);
                    GList *params_list = NULL;
                    struct Function *func = make_Function($1, identifier, params_list, $6);
                    $$ = func;
                }
                ;

statement:      assignment SEMI {
                    struct Statement *stmt = make_Statement(STMT_ASSIGN);
                    stmt->assignment = $1;
                    $$ = stmt;
                }
                | RETURN expr SEMI {
                    struct Statement_return *ret = make_Return($2);
                    struct Statement *stmt =make_Statement(STMT_RETURN);
                    stmt->ret = ret;
                    $$ = stmt;
                }
                ;

assignment:
                ID EQUALS expr {
                    struct Identifier *identifier = make_Identifier($1);
                    struct Statement_assignment *assignment = make_Assignment(identifier, $3);
                    $$ = assignment;
                }
                ;

expr:
                expr BINOP expr {
                    struct Expr_op *binop = make_Expr_Op($2, $1, $3);
                    struct Expr *expr = make_Expr(EXPR_OP);
                    expr->op = binop;
                    $$ = expr;
                }
        |
                expr RELOP expr {
                    struct Expr_op *relop = make_Expr_Op($2, $1, $3);
                    struct Expr *expr = make_Expr(EXPR_OP);
                    expr->op = relop;
                    $$ = expr;
                }
        |
                ID {
                    struct Identifier *identifier = make_Identifier($1);
                    struct Expr_identifier *expr_identifier = make_Expr_Identifier(identifier);
                    struct Expr *expr = make_Expr(EXPR_IDENTIFIER);
                    expr->id = expr_identifier;
                    $$ = expr;
                }
        |
                INTEGER {
                    struct Constant *constant = make_Constant($1);
                    struct Expr_constant *expr_constant = make_Expr_Constant(constant);
                    struct Expr *expr = make_Expr(EXPR_CONSTANT);
                    expr->constant = expr_constant;
                    $$ = expr;
                }
        ;

type:
                ID {
                    struct Type *type = make_Type($1);
                    $$ = type;
                }
                ;

%%

int yyerror (struct Program **p, const char *s) {
    fprintf(stderr, "%d: error: %s\n", yylineno, s);
    return 1;
}
