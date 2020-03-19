%{

#include <stdio.h>
#include <glib.h>
#include "util.h"
#include "lang.h"

#define YYDEBUG 1

int yylex();
int yyerror(struct Program **prog, const char *msg);
extern int yylineno;

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
    struct StatementSelection *selection;
    struct StatementJump *jump;
    struct Block *block;
    void *block_element;
}

%parse-param {struct Program **prog}

%start program
%token                  RELOP UNOP BINOP
%token                  ID
%token                  INTEGER
%token                  NEWLINE
%token                  RETURN
%token                  IF
%token                  ELSE

%type   <program>       program
%type   <integer>       INTEGER
%type   <op>            RELOP BINOP UNOP
%type   <block>         top_level_block block compound_statement
%type   <block_element> top_level_block_element block_element
%type   <statement>     statement
%type   <declaration>   declaration
%type   <type>          type
%type   <str>           ID
%type   <function>      function
%type   <expr>          expr expr_statement
%type   <jump>          jump_statement
%type   <selection>     selection_statement

%left EQUALS
%left RELOP
%left BINOP

%%

program:
                top_level_block {
                    struct Program *full_program = program_new($1);
                    *prog = full_program;

                }
                ;
top_level_block:
                top_level_block_element {
                    GList *list = NULL;
                    list = g_list_append(list, $1);
                    struct Block *block = block_new(list, NULL);
                    $$ = block;
                }
        |
                top_level_block top_level_block_element {
                    $$ = block_extend($1, $2);
                }
                ;

top_level_block_element:
                declaration { $$ = $1; }
        |
                function { $$ = $1; }
                ;

declaration:
                type ID ';' {
                    struct Identifier *identifier = identifier_new($2, $1);
                    struct Declaration *decl = declaration_new($1, identifier);
                    $$ = decl;
                }
                ;

block:
                block block_element {
                    $$ = block_extend($1, $2);
                }
        |
                block_element {
                    GList *list = NULL;
                    list = g_list_append(list, $1);
                    struct Block *block = block_new(list, NULL);
                    $$ = block;
                }
        ;

block_element:
                declaration { $$ = $1; }
        |       statement { $$ = $1; }
                ;

function:
                type ID '(' ')' '{' block '}' {
                    struct Identifier *identifier = identifier_new($2, NULL);
                    type_make_fn_type($1);
                    identifier->type = $1;
                    GList *params_list = NULL;
                    struct Function *func = function_new($1, identifier, params_list, $6);
                    $$ = func;
                }
                ;

statement:
                compound_statement {
                    struct Statement *stmt = statement_new(STMT_TYPE_COMPOUND);
                    stmt->compound = $1;
                    $$ = stmt;
                }
        |
                expr_statement {
                    struct Statement *stmt = statement_new(STMT_TYPE_EXPR);
                    stmt->expr = $1;
                    $$ = stmt;
                }
        |
                jump_statement {
                    struct Statement *stmt = statement_new(STMT_TYPE_JUMP);
                    stmt->jump = $1;
                    $$ = stmt;
                }
        |
                selection_statement {
                    struct Statement *stmt = statement_new(STMT_TYPE_SELECTION);
                    stmt->selection = $1;
                    $$ = stmt;
                }
                ;

compound_statement:
                '{' block '}' {
                    $$ = $2;
                }
                ;

expr_statement:
                expr ';' {
                    $$ = $1;
                }
                ;

jump_statement:
                RETURN expr ';' {
                    struct StatementJump *jump = statement_jump_new();
                    jump->type = JUMP_RETURN;
                    jump->expr = $2;
                    $$ = jump;
                }
                ;

selection_statement:
                IF '(' expr ')' statement {
                    struct StatementSelection *cond = statement_selection_new($3, $5, NULL);
                    $$ = cond;
                }
        |
                IF '(' expr ')' statement ELSE statement {
                    struct StatementSelection *cond = statement_selection_new($3, $5, $7);
                    $$ = cond;
                }
                ;

expr:
                expr BINOP expr {
                    struct ExprOp *binop = expr_op_new($2, $1, $3);
                    struct Expr *expr = expr_new(EXPR_BINOP);
                    expr->op = binop;
                    $$ = expr;
                }
        |
                expr RELOP expr {
                    struct ExprOp *relop = expr_op_new($2, $1, $3);
                    struct Expr *expr = expr_new(EXPR_RELOP);
                    expr->op = relop;
                    $$ = expr;
                }
        |
                ID {
                    struct Identifier *identifier = identifier_new($1, NULL);
                    struct ExprIdentifier *expr_identifier = expr_identifier_new(identifier);
                    struct Expr *expr = expr_new(EXPR_IDENTIFIER);
                    expr->id = expr_identifier;
                    $$ = expr;
                }
        |
                INTEGER {
                    struct Constant *constant = constant_new($1);
                    struct ExprConstant *expr_constant = expr_constant_new(constant);
                    struct Expr *expr = expr_new(EXPR_CONSTANT);
                    expr->constant = expr_constant;
                    $$ = expr;
                }
        |
                ID '=' expr {
                    struct Identifier *identifier = identifier_new($1, NULL);
                    struct ExprAssignment *assignment = expr_assignment_new(identifier, $3);
                    struct Expr *expr = expr_new(EXPR_ASSIGNMENT);
                    expr->assignment = assignment;
                    $$ = expr;
                }
                ;
type:
                ID {
                    struct Type *type = type_new($1);
                    $$ = type;
                }
                ;

%%

int yyerror (struct Program **p, const char *s) {
    fprintf(stderr, "%d: error: %s\n", yylineno, s);
    return 1;
}
