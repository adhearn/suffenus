#include <stdio.h>
#include <stdlib.h>
#include "lang.h"
#include "list.h"
#include "util.h"

struct Program *make_Program(List *stat_list) {
    struct Program *prog = malloc(sizeof(struct Program));
    check_mem(prog);

    prog->stat_list = stat_list;
    return prog;
}

struct Identifier *make_Identifier(char *id) {
    struct Identifier *ident = malloc(sizeof(struct Identifier));
    check_mem(id);

    ident->id = id;
    return ident;
}

struct Constant *make_Constant(int val) {
    struct Constant *c = malloc(sizeof(struct Identifier));
    check_mem(c);

    c->val = val;
    return c;
}

struct Exp_unary *make_Exp_unary(struct Identifier *id) {
    struct Exp_unary *unary = malloc(sizeof(struct Exp_unary));
    check_mem(unary);

    unary->id = id;
    return unary;
}

struct Assignment_rhs *make_Rhs_constant(struct Constant *constant) {
    struct Assignment_rhs *rhs = malloc(sizeof(struct Assignment_rhs));
    check_mem(rhs);

    rhs->type = RHS_CONSTANT;
    rhs->op = Op_null;
    rhs->val1.constant = constant;
    return rhs;
}

struct Assignment_rhs *make_Rhs_identifier(struct Identifier *id) {
    struct Assignment_rhs *rhs = malloc(sizeof(struct Assignment_rhs));
    check_mem(rhs);

    rhs->type = RHS_IDENTIFIER;
    rhs->op = Op_null;
    rhs->val1.identifier = id;
    return rhs;
}

struct Assignment_rhs *make_Rhs_binary(struct Assignment_rhs *val1, struct Assignment_rhs *val2, enum Op op) {
    struct Assignment_rhs *rhs = malloc(sizeof(struct Assignment_rhs));
    check_mem(rhs);

    rhs->type = RHS_BINARY;
    rhs->op = op;
    rhs->val1.val1_rhs = val1;
    rhs->val2 = val2;
    return rhs;
}

struct Exp_assignment *make_Exp_assignment(struct Exp_unary *lval, struct Assignment_rhs *rhs) {
    struct Exp_assignment *exp = malloc(sizeof(struct Assignment_rhs));
    check_mem(rhs);

    exp->lval = lval;
    exp->rhs = rhs;
    return exp;
}

struct Exp *make_Exp(struct Exp_assignment *assign) {
    struct Exp *exp = malloc(sizeof(struct Exp));
    check_mem(exp);

    exp->assignment = assign;
    return exp;
}

struct Statement_select *make_Statement_select(struct Exp *conditional,
                                               struct Statement *conseq,
                                               struct Statement *alt) {
    struct Statement_select *stat = malloc(sizeof(struct Statement_select));
    check_mem(stat);

    stat->conditional = conditional;
    stat->conseq = conseq;
    stat->alt = alt;
    return stat;
}

struct Statement_jump *make_Statement_jump(struct Exp *exp) {
    struct Statement_jump *jmp = malloc(sizeof(struct Statement_jump));
    check_mem(jmp);

    jmp->return_val = exp;
    return jmp;
}

struct Statement *make_select_Statement(struct Statement_select *sel) {
    struct Statement *stat = malloc(sizeof(struct Statement));
    check_mem(stat);

    stat->type = STAT_SELECTION;
    stat->statement.select = sel;

    return stat;
}

struct Statement *make_jump_Statement(struct Statement_jump *jump) {
    struct Statement *stat = malloc(sizeof(struct Statement));
    check_mem(stat);

    stat->type = STAT_JUMP;
    stat->statement.jump = jump;

    return stat;
}

struct Statement *make_exp_Statement(struct Statement_exp *exp) {
    struct Statement *stat = malloc(sizeof(struct Statement));
    check_mem(stat);

    stat->type = STAT_EXP;
    stat->statement.exp = exp;

    return stat;
}

struct Primary_exp *make_Primary_constant(struct Constant *constant) {
    struct Primary_exp *exp = malloc(sizeof(struct Primary_exp));
    check_mem(exp);

    exp->type = Primary_constant;
    exp->val.constant = constant;
    return exp;
}

struct Primary_exp *make_Primary_identifier(struct Identifier *identifier) {
    struct Primary_exp *exp = malloc(sizeof(struct Primary_exp));
    check_mem(exp);

    exp->type = Primary_id;
    exp->val.id = identifier;
    return exp;
}

struct Assignment_rhs *primary_to_rhs(struct Primary_exp *prim) {
    struct Assignment_rhs *ret;

    switch (prim->type) {
        case Primary_id:
            ret = make_Rhs_identifier(prim->val.id);
            break;
        case Primary_constant:
            ret = make_Rhs_constant(prim->val.constant);
            break;
    }

    return ret;
}

struct Exp_unary *primary_to_unary(struct Primary_exp *prim) {
    switch (prim->type) {
    case Primary_id:
        return make_Exp_unary(prim->val.id);
        break;
    case Primary_constant:
        return NULL;
        break;
    }

    return NULL;
}
