%{
#include <stdio.h>
#include "compiler.h"
#include "lang.h"
#include "list.h"
#define YYDEBUG 1

int yylex();
int yyerror();
%}

%union {
    int integer;
    char *str;
    List *list;
    enum Op op;
    struct Program *program;
    struct Statement *stat;
    struct Statement_jump *stat_jump;
    struct Statement_exp *stat_exp;
    struct Statement_select * stat_select;
    struct Exp *exp;
    struct Exp_assignment *exp_assignment;
    struct Assignment_rhs *rhs;
    struct Constant *constant;
    struct Identifier *identifier;
    struct Primary_exp *primary;
    struct Exp_unary *unary;
}

%start program
%token IF ELSE
%token RETURN
%token EQUALITY
%token CMP
%token SHIFT
%token AND OR
%token ID
%token INTEGER

%type   <integer>       INTEGER
%type   <str>           ID
%type   <op>            SHIFT EQUALITY CMP
%type   <list>          stat_list
%type   <program>       program
%type   <stat>          stat
%type   <stat_jump>     jump_stat
%type   <stat_exp>      exp_stat
%type   <stat_select>   selection_stat
%type   <exp>           exp
%type   <exp_assignment> assignment_exp
%type   <rhs>           conditional_exp logical_or_exp logical_and_exp inclusive_or_exp equality_exp relational_exp shift_expression additive_exp mult_exp
%type   <constant>      const_exp
%type   <primary>       primary_exp
%type   <unary>         unary_exp
%%

program:        stat_list {
                    printf("program stat_list\n");
                    $$ = make_Program($1);
                }
                ;

stat_list:      stat {
                    printf("stat_list with single stat\n");
                    List *stats = List_new(free);
                    List_prepend(stats, $1);
                    $$ = stats;
                }
        |       stat_list stat {
            printf("stat_list with recursion");
            List_append($1, $2);
            $$ = $1;
                }
        ;

stat:           jump_stat {
    printf("jump_stat\n");
    struct Statement *stat = make_jump_Statement($1);
    $$ = stat;
 }
        |       selection_stat {
            printf("selection_stat\n");
            struct Statement *stat = make_select_Statement($1);
            $$ = stat;
                }
        |       exp_stat {
            printf("exp_stat\n");
            struct Statement *stat = make_exp_Statement($1);
            $$ = stat;
                }
        ;

exp_stat:       exp ';' { printf("exp;\n"); }
        |       ';' { printf(";\n");}
        ;

selection_stat: IF '(' exp ')' stat { $$ = make_Statement_select($3, $5, NULL); }
        |       IF '(' exp ')' stat ELSE stat { $$ = make_Statement_select($3, $5, $7); }
        ;

jump_stat:      RETURN exp ';' {
                        $$ = make_Statement_jump($2);
                }
        |       RETURN ';' {
            $$ = make_Statement_jump(NULL);
                }
        ;

exp:            assignment_exp {
                    $$ = make_Exp($1);
                }
        ;

assignment_exp: conditional_exp { $$ = make_Exp_assignment(NULL, $1); }
        |       unary_exp assignment_operator conditional_exp {
            $$ = make_Exp_assignment($1, $3);
                }
        ;

assignment_operator:
                '=' { }
        ;

conditional_exp:
                logical_or_exp { $$ = $1; }
        ;

logical_or_exp: logical_and_exp { $$ = $1; }
        |       logical_or_exp OR logical_and_exp {
            $$ = make_Rhs_binary($1, $3, Op_or);
                }
        ;

logical_and_exp:
                inclusive_or_exp  { $$ = $1; }
        |       logical_and_exp AND inclusive_or_exp {
            $$ = make_Rhs_binary($1, $3, Op_and);
                }
        ;

inclusive_or_exp:
                equality_exp { $$ = $1; }
        ;

equality_exp:   relational_exp { $$ = $1; }
        |       equality_exp EQUALITY relational_exp {
$$ = make_Rhs_binary($1, $3, $2);
                }
        ;

relational_exp: shift_expression { $$ = $1; }
        |       relational_exp CMP shift_expression {
            printf("relational_exp CMP shift_expression\n");
                }
        ;

shift_expression:
                additive_exp { $$ = $1; }
        |       shift_expression SHIFT additive_exp {
$$ = make_Rhs_binary($1, $3, $2);
                }
                ;

additive_exp:   mult_exp { $$ = $1; }
        |       additive_exp '+' mult_exp { make_Rhs_binary($1, $3, Op_add); }
        |       additive_exp '-' mult_exp { make_Rhs_binary($1, $3, Op_sub); }
        ;

mult_exp:       primary_exp { $$ = primary_to_rhs($1); }
        |       mult_exp '*' primary_exp {
            struct Assignment_rhs *rhs = primary_to_rhs($3);
            make_Rhs_binary($1, rhs, Op_mult);
                }
        |       mult_exp '/' primary_exp {
            struct Assignment_rhs *rhs = primary_to_rhs($3);
            make_Rhs_binary($1, rhs, Op_div); }
        |       mult_exp '%' primary_exp {
struct Assignment_rhs *rhs = primary_to_rhs($3);
make_Rhs_binary($1, rhs, Op_mod);
                }
        ;

unary_exp:      primary_exp { $$ = primary_to_unary($1); }
        ;

primary_exp:    ID { struct Identifier *id = make_Identifier($1);
                    $$ = make_Primary_identifier(id);
                }
        |       const_exp { $$ = make_Primary_constant($1); }
        ;

const_exp:      INTEGER { $$ = make_Constant($1); }
        ;

%%

int yyerror (char *s) {
    fprintf(stderr, "error: %s\n", s);
    return 1;
}
