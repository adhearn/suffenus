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
    struct Constant *constant;
    struct Label *label;
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
%type   <str>           ID
%type   <label>         label
/* %type   <primary>       primary_exp */
/* %type   <unary>         unary_exp */

%%

program:        statement_list {
                    struct Program *full_program = make_Program($1);
                    *prog = full_program;
 }
                ;

statement_list:      statement {
                    List *stats = List_new(free);
                    List_prepend(stats, $1);
                    $$ = stats;
                }
        |       statement_list statement {
            List_append($1, $2);
            $$ = $1;
                }
        ;

// Construct a label table to hold the address a label points to
statement:      label SEMI stat NEWLINE {
                    $3->label = $1;
                    $$ = $3;
 }
        |       stat NEWLINE { $$ = $1; }
        ;

stat:           assignment { $$ = $1; }
        |       jump { $$ = $1; }
        |       conditional_jump { $$ = $1; }
        ;

assignment:     address '=' assignment_rhs {
                    struct Quad *quad = $3;
                    quad->result = $1;
                    $$ = quad;
                }
        ;

assignment_rhs: UNOP address { $$ = make_Quad(NULL, $1, $2, NULL); }
        |       address BINOP address { $$ = make_Quad(NULL, $2, $1, $3); }
        |       address { $$ = make_Quad(NULL, Op_null, $1, NULL); }
        ;

jump:           JUMP label {
                    struct Address *addrLabel = make_Address_Label($2);
                    $$ = make_Quad(addrLabel, Op_jump, NULL, NULL); }
        ;

conditional_jump:
                IF conditional JUMP label {
                    struct Quad *quad = $2;
                    struct Address *addrLabel = make_Address_Label($4);
                    quad->result = addrLabel;
                    $$ = quad;
                }
        ;

conditional:    address { $$ = make_Quad(NULL, Op_null, $1, NULL); }
        |       address RELOP address { $$ = make_Quad(NULL, $2, $1, $3); }
        ;

address:        INTEGER {
                    struct Constant *constant = make_Constant($1);
                    struct Address *addr = make_Address_Constant(constant);
                    $$ = addr;
                }
        |       ID {
            struct Identifier *id = make_Identifier($1);
            $$ = make_Address_Identifier(id);
                }
        ;

label:          ID { struct Label *label = make_Label($1);
                    $$ = label;
                    //$$ = make_Address_Label(label);
                }
        ;

%%

int yyerror (char *s) {
    fprintf(stderr, "error: %s\n", s);
    return 1;
}
