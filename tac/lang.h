#ifndef _lang_h
#define _lang_h

#include "list.h"

struct Program {
    List *stat_list;
};

struct Identifier {
    char *id;
};

struct Constant {
    int val;
};

struct Exp_unary {
    struct Identifier *id;
};


enum Rhs_type {
    RHS_UNARY,
    RHS_BINARY,
    RHS_CONSTANT,
    RHS_IDENTIFIER
};

enum Op {
    Op_null,
    Op_or,
    Op_and,
    Op_add,
    Op_sub,
    Op_mult,
    Op_div,
    Op_mod,
    Op_lshift,
    Op_rshift,
    Op_eq,
    Op_neq,
    Op_lt,
    Op_gt,
    Op_lte,
    Op_gte
};

enum Primary_type {
    Primary_id,
    Primary_constant
};

struct Primary_exp {
    enum Primary_type type;
    union {
        struct Identifier *id;
        struct Constant *constant;
    } val;
};

struct Assignment_rhs {
    enum Rhs_type type;
    enum Op op;
    union {
        struct Assignment_rhs *val1_rhs;
        struct Identifier *identifier;
        struct Constant *constant;
    } val1;
    struct Assignment_rhs *val2;
};

struct Exp_assignment {
    struct Exp_unary *lval;
    struct Exp_assignment *rhs;
};

struct Exp {
    struct Exp_assignment *assignment;
};

enum Statement_type {
    STAT_JUMP,
    STAT_SELECTION,
    STAT_EXP
};

struct Statement_jump {
    struct Exp *return_val;
};

struct Statement_select {
    struct Exp *conditional;
    struct Statement *conseq;
    struct Statement *alt;
};

struct Statement_exp {
    struct Exp *exp;
};

struct Statement {
    enum Statement_type type;
    union {
        struct Statement_jump *jump;
        struct Statement_select *select;
        struct Statement_exp *exp;
    } statement;
};

struct Program *make_Program(List *stat_list);
struct Exp_unary *make_Exp_unary(struct Identifier *id);
struct Assignment_rhs *make_Rhs_constant(struct Constant *constant);
struct Assignment_rhs *make_Rhs_identifier(struct Identifier *id);
struct Assignment_rhs *make_Rhs_binary(struct Assignment_rhs *val1, struct Assignment_rhs *val2, enum Op op);
struct Exp_assignment *make_Exp_assignment(struct Exp_unary *lval, struct Assignment_rhs *rhs);
struct Exp *make_Exp(struct Exp_assignment *assign);
struct Statement *make_select_Statement(struct Statement_select *sel);
struct Statement *make_jump_Statement(struct Statement_jump *jump);
struct Statement *make_exp_Statement(struct Statement_exp *exp);
struct Statement_select *make_Statement_select(struct Exp *conditional, struct Statement *conseq, struct Statement *alt);
struct Statement_jump *make_Statement_jump(struct Exp *return_val);
struct Identifier *make_Identifier(char *id);
struct Constant *make_Constant(int val);
struct Primary_exp *make_Primary_identifier(struct Identifier *id);
struct Primary_exp *make_Primary_constant(struct Constant *constant);
struct Assignment_rhs *primary_to_rhs(struct Primary_exp *prim);
struct Exp_unary *primary_to_unary(struct Primary_exp *prim);
#endif
