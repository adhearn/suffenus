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
    struct Declarator *declarator;
    struct Declaration *declaration;
    struct Expr *expr;
    struct Statement *statement;
    struct StatementFor *for_loop;
    struct StatementJump *jump;
    struct StatementLabeled *labeled;
    struct StatementSelection *selection;
    struct StatementSwitch *switch_statement;
    struct StatementWhile *while_loop;
    struct Program *program;
    struct Function *function;
    struct Type *type;
    struct Block *block;
    void *block_element;
}

%parse-param {struct Program **prog}

%start program
%token                  RELOP UNOP BINOP
%token                  EQOP
%token                  ID
%token                  INTEGER
%token                  NEWLINE
%token                  RETURN
%token                  IF ELSE
%token                  FOR WHILE
%token                  BREAK CONTINUE GOTO
%token                  SWITCH CASE DEFAULT
%token                  INT VOID

%type   <program>       program
%type   <integer>       INTEGER type_specifier INT VOID
%type   <op>            RELOP BINOP UNOP EQOP
%type   <block>         top_level_block block compound_statement
%type   <block_element> top_level_block_element block_element
%type   <statement>     statement
%type   <declarator>    declarator direct_declarator init_declarator
%type   <declaration>   declaration parameter_declaration
%type   <str>           ID
%type   <identifier>    identifier
%type   <function>      function
%type   <list>          argument_expr_list declaration_specifiers identifier_list parameter_type_list init_declarator_list parameter_list
%type   <expr>          expr expr_statement primary_expr postfix_expr unary_expr relational_expr additive_expr multiplicative_expr assignment_expr equality_expr and_expr exclusive_or_expr inclusive_or_expr logical_and_expr logical_or_expr conditional_expr constant_expr initializer
%type   <for_loop>      for_statement
%type   <jump>          jump_statement
%type   <labeled>       labeled_statement
%type   <switch_statement> switch_statement
%type   <while_loop>    while_statement
%type   <selection>     selection_statement

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
                declaration_specifiers ';' {
                    $$ = declaration_new($1, NULL);
                }
        |
                declaration_specifiers init_declarator_list ';' {
                    $$ = declaration_new($1, $2);
                }
        ;

declaration_specifiers:
                type_specifier {
                    $$ = g_list_append(NULL, GINT_TO_POINTER($1));
                }
        |
                declaration_specifiers type_specifier {
                    $$ = g_list_prepend($1, GINT_TO_POINTER($1));
                }
                ;

init_declarator_list:
                init_declarator {
                    $$ = g_list_append(NULL, $1);
                }
        |
                init_declarator_list ',' init_declarator {
                    $$ = g_list_append($1, $3);
                }
        ;

init_declarator:
                declarator { $$ = $1; }
        |
                declarator '=' initializer {
                    $1->initializer = $3;
                    $$ = $1;
                }
        ;

initializer:
                assignment_expr { $$ = $1; }
        /* | */
        /*         '{' initializer_list '}' { */

        /*         } */
        /* | */
        /*         '{' initializer_list ',' '}' { */

        /*         } */
        ;

// For array/struct initialization later
/* initializer_list: */
/*                 initializer {} */
/*         |       initializer_list ',' initializer {} */
/* ; */

declarator:
                direct_declarator { $$ = $1; }
                ;

direct_declarator:
                identifier { $$ = declarator_new(DECLARATOR_IDENTIFIER, NULL, $1); }
        |
                '(' declarator ')' { $$ = $2; }
        |
                direct_declarator '[' constant_expr ']' {
                    $$ = declarator_new(DECLARATOR_ARRAY, $1, $3);
                }
        |
                direct_declarator '[' ']' {
                    $$ = declarator_new(DECLARATOR_ARRAY, $1, NULL);
                }
        |
                direct_declarator '(' parameter_type_list ')' {
                    $$ = declarator_new(DECLARATOR_FUNCTION, $1, $3);
                }
        |
                direct_declarator '(' identifier_list ')' {
                    $$ = declarator_new(DECLARATOR_FUNCTION, $1, $3);
                }
        |
                direct_declarator '(' ')' {
                    $$ = declarator_new(DECLARATOR_FUNCTION, $1, NULL);
                }
                ;

/* declaration_specifier: */
/*                 type ID { */
/*                     struct Identifier *identifier = identifier_new($2, $1); */
/*                     struct Declaration *decl = declaration_new($1, identifier); */
/*                     $$ = decl; */
/*                 } */
/*                 ; */


type_specifier:
                INT { $$ = $1; }
        |
                VOID {$$ = $1; }
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
        |
                statement { $$ = $1; }
                ;

parameter_type_list:
                parameter_list { $$ = $1; }
        ;

parameter_list:
                parameter_declaration {
                    $$ = g_list_append(NULL, $1);
                }
        |
                parameter_list parameter_declaration {
                    $$ = g_list_append($1, $2);
                }
        ;

parameter_declaration:
                declaration_specifiers declarator {
                    GList *declarators = g_list_append(NULL, $2);
                    $$ = declaration_new($1, declarators);
                }
        |
                declaration_specifiers {
                    $$ = declaration_new($1, NULL);
                }
        ;

function:
                declaration_specifiers declarator compound_statement {
                    struct Function *func = function_new($2, $3, $1);
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
                for_statement {
                    struct Statement *stmt = statement_new(STMT_TYPE_FOR);
                    stmt->for_loop = $1;
                    $$ = stmt;
                }
        |
                jump_statement {
                    struct Statement *stmt = statement_new(STMT_TYPE_JUMP);
                    stmt->jump = $1;
                    $$ = stmt;
                }
        |
                labeled_statement {
                    struct Statement *stmt = statement_new(STMT_TYPE_LABELED);
                    stmt->labeled = $1;
                    $$ = stmt;
                }
|
                selection_statement {
                    struct Statement *stmt = statement_new(STMT_TYPE_SELECTION);
                    stmt->selection = $1;
                    $$ = stmt;
                }
        |
                switch_statement {
                    struct Statement *stmt = statement_new(STMT_TYPE_SWITCH);
                    stmt->switch_statement = $1;
                    $$ = stmt;
                }
        |
                while_statement {
                    struct Statement *stmt = statement_new(STMT_TYPE_WHILE);
                    stmt->while_loop = $1;
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

for_statement:
                FOR '(' expr_statement expr_statement ')' statement {
                    $$ = statement_for_new($3, $4, NULL, $6);
                }
        |
                FOR '(' expr_statement expr_statement expr ')' statement {
                    $$ = statement_for_new($3, $4, $5, $7);
                }
                ;

jump_statement:
                BREAK ';' {
                    struct StatementJump *jump = statement_jump_new(JUMP_BREAK);
                    $$ = jump;
                }
        |
                CONTINUE ';' {
                    struct StatementJump *jump = statement_jump_new(JUMP_CONTINUE);
                    $$ = jump;
                }
        |
                GOTO identifier ';' {
                    struct StatementJump *jump = statement_jump_new(JUMP_GOTO);
                    jump->identifier = $2;
                    $$ = jump;
                }
        |
                RETURN expr ';' {
                    struct StatementJump *jump = statement_jump_new(JUMP_RETURN);
                    jump->expr = $2;
                    $$ = jump;
                }
                ;

labeled_statement:
                identifier ':' statement {
                    struct StatementLabeled *labeled = statement_labeled_new(LABELED_LABEL, $1, $3);
                    $$ = labeled;
                }
        |
                CASE expr ':' statement {
                    struct StatementLabeled *labeled = statement_labeled_new(LABELED_CASE, $2, $4);
                    $$ = labeled;
                }
        |
                DEFAULT ':' statement {
                    struct StatementLabeled *labeled = statement_labeled_new(LABELED_DEFAULT, NULL, $3);
                    $$ = labeled;
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

// TODO: This doesn't actually parse case statements the way the standard says it should - my parser requires
// the body of the case statement to be surround by braces. Otherwise, we can't know what to do with statements
// beyond the first statement in a case body - does it belong to the case statement, or as a new element in
// the switch statement body? I don't actually know how other parsers solve this problem. Again, my lack
// of parser theory is probably hurting me here.
switch_statement:
                SWITCH '(' expr ')' statement {
                    struct StatementSwitch *sel = statement_switch_new($3, $5);
                    $$ = sel;
                }
                ;

while_statement:
                WHILE '(' expr ')' statement {
                    $$ = statement_while_new($3, $5);
                }
                ;


primary_expr:
                identifier {
                    struct ExprIdentifier *expr_identifier = expr_identifier_new($1);
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
                ;

argument_expr_list:
                assignment_expr {
                    GList *expr_list = g_list_append(NULL, $1);
                    $$ = expr_list;
                }
        |
                argument_expr_list ',' assignment_expr {
                    GList *expr_list = g_list_append($1, $3);
                    $$ = expr_list;
                }
        ;

postfix_expr:
                primary_expr { $$ = $1; }
        |
                postfix_expr '(' ')' {
                    struct ExprCall *call = expr_call_new($1, NULL);
                    struct Expr *expr = expr_new(EXPR_CALL);
                    expr->call = call;
                    $$ = expr;
                }
        |
                postfix_expr '(' argument_expr_list ')' {
                    struct ExprCall *call = expr_call_new($1, $3);
                    struct Expr *expr = expr_new(EXPR_CALL);
                    expr->call = call;
                    $$ = expr;
                }
        |
                postfix_expr '[' expr ']' {

                }
        ;

unary_expr:
                postfix_expr { $$ = $1; }
        ;

multiplicative_expr:
                unary_expr { $$ = $1; }
        |
                multiplicative_expr '*' unary_expr {
                    struct ExprOp *binop = expr_op_new(Op_mul, $1, $3);
                    struct Expr *expr = expr_new(EXPR_BINOP);
                    expr->op = binop;
                    $$ = expr;
                }
        |
                multiplicative_expr '/' unary_expr {
                    struct ExprOp *binop = expr_op_new(Op_div, $1, $3);
                    struct Expr *expr = expr_new(EXPR_BINOP);
                    expr->op = binop;
                    $$ = expr;
                }
;


additive_expr:
                multiplicative_expr { $$ = $1; }
        |
                additive_expr '+' multiplicative_expr {
                    struct ExprOp *binop = expr_op_new(Op_add, $1, $3);
                    struct Expr *expr = expr_new(EXPR_BINOP);
                    expr->op = binop;
                    $$ = expr;
                }
        |
                additive_expr '-' multiplicative_expr {
                    struct ExprOp *binop = expr_op_new(Op_sub, $1, $3);
                    struct Expr *expr = expr_new(EXPR_BINOP);
                    expr->op = binop;
                    $$ = expr;
                }
                ;

relational_expr:
                additive_expr { $$ = $1; }
        |
                relational_expr RELOP additive_expr {
                    struct ExprOp *relop = expr_op_new($2, $1, $3);
                    struct Expr *expr = expr_new(EXPR_RELOP);
                    expr->op = relop;
                    $$ = expr;
                }
        ;

equality_expr:
                relational_expr { $$ = $1; }
        |
                equality_expr EQOP relational_expr {
                    struct ExprOp *relop = expr_op_new($2, $1, $3);
                    struct Expr *expr = expr_new(EXPR_RELOP);
                    expr->op = relop;
                    $$ = expr;
                }
        ;

and_expr:
                equality_expr { $$ = $1; }
        //|       and_expr '&' equality_expr
                ;

exclusive_or_expr:
                and_expr { $$ = $1; }
//              | exclusive_or_expr '^' and_expr
                ;

inclusive_or_expr:
                exclusive_or_expr { $$ = $1; }
//                      |       inclusive_or_expr '|' exclusive_or_expr
                ;

logical_and_expr:
                inclusive_or_expr { $$ = $1; }
//              | logical_and_expr AND_OP inclusive_or_expr
                ;

logical_or_expr:
                logical_and_expr { $$ = $1; }
//        |       logical_or_expr OR_OP logical_and_expr
                ;

conditional_expr:
                logical_or_expr { $$ = $1; }
        ;

constant_expr:
                conditional_expr { $$ = $1; }
        ;

assignment_expr:
                equality_expr { $$ = $1; }
        |
                identifier '=' assignment_expr {
                    struct ExprAssignment *assignment = expr_assignment_new(LVALUE_IDENTIFIER, $1, $3);
                    struct Expr *expr = expr_new(EXPR_ASSIGNMENT);
                    expr->assignment = assignment;
                    $$ = expr;
                }
        |
                postfix_expr '=' assignment_expr {
                    struct Expr *postfix = $1;
                    if (postfix->type != EXPR_INDEXED) {
                        log_err("Only Identifiers and indexed Expressions can appear on the LHS of an assignment");
                        YYABORT;
                    } else {
                        struct ExprAssignment *assignment = expr_assignment_new(LVALUE_INDEXED, $1, $3);
                        struct Expr *expr = expr_new(EXPR_ASSIGNMENT);
                        expr->assignment = assignment;
                        $$ = expr;
                    }
                }
        ;

expr:           assignment_expr { $$ = $1; }
        ;

identifier_list:
                identifier {
                    $$ = g_list_append(NULL, $1);
                }
        |
                identifier_list ',' identifier {
                    $$ = g_list_append($1, $3);
                }
        ;

identifier:
                ID {
                    struct Identifier *id = identifier_new($1, NULL);
                    $$ = id;
                }
        ;

%%

int yyerror (struct Program **p, const char *s) {
    fprintf(stderr, "%d: error: %s\n", yylineno, s);
    return 1;
}
