%{
#include <stdio.h>
#include "compiler.h"
#include "tac.h"
#include "list.h"
#include "util.h"
#define YYDEBUG 1

int yylex();
int yyerror();
%}

%union {
    int integer;
    char *str;
    List *list;
    enum Op op;
    struct Quad *quad;
    struct Program *program;
    struct Address *addr;
    struct Identifier *id;
    struct Constant *constant;
}

%parse-param {struct Program **prog}

%start program
%token IF
//                      %token RETURN
%token RELOP UNOP BINOP
%token JUMP
%token ID
%token INTEGER
%token LABEL
%token SEMI
%token NEWLINE


%type   <integer>       INTEGER
%type   <op>            RELOP BINOP UNOP
%type   <list>          statement_list
%type   <program>       program
%type   <addr>          address
%type   <quad>          stat statement assignment assignment_rhs jump conditional_jump conditional
%type   <id>            label
%type   <str>           ID
/* %type   <primary>       primary_exp */
/* %type   <unary>         unary_exp */

%%

program:        statement_list {
                    printf("Program: statement_list\n");
                    struct Program *full_program = make_Program($1);
                    *prog = full_program;
 }
                ;

statement_list:      statement {
    printf("statement_list without recursion\n");
                    List *stats = List_new(free);
                    List_prepend(stats, $1);
                    $$ = stats;
                }
        |       statement_list statement {
            printf("statement_list with recursion\n");
            List_append($1, $2);
            $$ = $1;
                }
        ;

// Construct a label table to hold the address a label points to
statement:      label SEMI stat NEWLINE {
                    printf("statement with label\n");
                    $3->label = $1;
                    $$ = $3;
 }
        |       stat NEWLINE { printf("statement without label\n"); $$ = $1; }
        ;

stat:           assignment { printf("assignment\n"); $$ = $1; }
        |       jump { printf("jump \n"); $$ = $1; }
        |       conditional_jump { printf("conditional jump statement\n"); $$ = $1; }
        ;

assignment:     address '=' assignment_rhs {
                    printf("assignment to address\n");
                    struct Quad *quad = $3;
                    quad->result = $1;
                    $$ = quad;
                }
        ;

assignment_rhs: UNOP address { printf("assignment unop\n"); $$ = make_Quad(NULL, $1, $2, NULL); }
        |       address BINOP address { printf("assignment binop\n"); $$ = make_Quad(NULL, $2, $1, $3); }
        |       address { printf("assignment copy\n");
     printf("Source address type: %d\n", $1->type);
     $$ = make_Quad(NULL, Op_null, $1, NULL); }
        ;

jump:           JUMP label { printf("unconditional jump\n"); $$ = make_Quad($2, Op_jump, NULL, NULL); }
        ;

conditional_jump:
                IF conditional JUMP label {
                    struct Quad *quad = $2;
                    quad->result = $4;
                    $$ = quad;
                }
        ;

conditional:    address { printf("conditional address\n"); $$ = make_Quad(NULL, Op_null, $1, NULL); }
        |       address RELOP address {
            printf("conditional relop\n");
            $$ = make_Quad(NULL, $2, $1, $3);
                }
        ;

address:        INTEGER {
    printf("INTEGER: %d\n", $1);
                    struct Constant *constant = make_Constant($1);
                    struct Address *addr = make_Address_Constant(constant);
                    printf("Address created; Type: %d\n", addr->type);
                    $$ = addr;
                }
        |       ID {
            struct Identifier *id = make_Identifier($1);
            $$ = make_Address_Identifier(id);
                }
        ;

label:          ID {
                    printf("label\n");
                    $$ = make_Identifier($1);
                    }
        ;

%%

int yyerror (char *s) {
    fprintf(stderr, "error: %s\n", s);
    return 1;
}
