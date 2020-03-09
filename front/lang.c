#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "lang.h"
#include "util.h"

struct Type *make_Type(char *type) {
    struct Type *t = malloc(sizeof(struct Type));
    check_mem(t);
    t->type = type;
    return t;
}

void free_Type(struct Type *type) {
    free(type->type);
    free(type);
}

void print_Type(struct Type *type) {
    printf("%s", type->type);
}

struct Identifier *make_Identifier(char *id) {
    struct Identifier *ident = malloc(sizeof(struct Identifier));
    check_mem(id);

    ident->id = id;
    return ident;
}

void free_Identifier(struct Identifier *id) {
    free(id->id);
    free(id);
}

void print_Identifier(struct Identifier *id) {
    printf("%s", id->id);
}

struct Constant *make_Constant(int constant) {
    struct Constant *c = malloc(sizeof(struct Identifier));
    check_mem(c);

    c->val = constant;
    return c;
}

void free_Constant(struct Constant *c) {
    free(c);
}

void print_Constant(struct Constant *c) {
    printf("%d", c->val);
}

struct Declaration *make_Declaration(struct Type *type, struct Identifier *id) {
    struct Declaration *decl = malloc(sizeof(struct Declaration));
    check_mem(decl);

    decl->type = type;
    decl->id = id;
    return decl;
}

void free_Declaration(struct Declaration *decl) {
    free_Type(decl->type);
    free_Identifier(decl->id);
    free(decl);
}

void print_Declaration(struct Declaration *decl) {
    printf("Declaration: %s %s\n", decl->type->type, decl->id->id);
}


struct Expr_op *make_Expr_Op(enum Op op, struct Expr *arg1, struct Expr *arg2) {
    struct Expr_op *expr_op = malloc(sizeof(struct Expr_op));
    check_mem(expr_op);

    expr_op->op = op;
    expr_op->arg1 = arg1;
    expr_op->arg2 = arg2;

    return expr_op;

    /* struct Expr *expr = malloc(sizeof(struct Expr)); */
    /* check_mem(expr); */
    /* expr->type = EXPR_OP; */
    /* expr->op = expr_op; */
    /* return expr; */
}

void free_Expr_op(struct Expr_op *expr) {
    free(expr->arg1);
    free(expr->arg2);
    free(expr);
}

struct Expr_identifier *make_Expr_Identifier(struct Identifier *id) {
    struct Expr_identifier *expr_id = malloc(sizeof(struct Expr_identifier));
    check_mem(expr_id);

    expr_id->id = id;
    return expr_id;

    /* struct Expr *expr = malloc(sizeof(struct Expr)); */
    /* check_mem(expr); */
    /* expr->id = expr_id; */
    /* return expr; */
}

void free_Expr_identifier(struct Expr_identifier *expr) {
    free(expr->id);
    free(expr);
}

struct Expr_constant *make_Expr_Constant(struct Constant *constant) {
    struct Expr_constant *expr_constant = malloc(sizeof(struct Expr_constant));
    check_mem(expr_constant);
    expr_constant->constant = constant;

    return expr_constant;

    /* struct Expr *expr = malloc(sizeof(struct Expr)); */
    /* check_mem(expr); */
    /* expr->constant = expr_constant; */
    /* return expr; */
}

void free_Expr_constant(struct Expr_constant *expr) {
    free(expr->constant);
    free(expr);
}

struct Expr *make_Expr(enum Expr_type type) {
    struct Expr *expr = malloc(sizeof(struct Expr));
    check_mem(expr);
    expr->type = type;
    return expr;
}

void free_Expr(struct Expr *expr) {
    switch (expr->type) {
    case (EXPR_CONSTANT):
        free_Expr_constant(expr->constant);
        break;
    case (EXPR_IDENTIFIER):
        free_Expr_identifier(expr->id);
        break;
    case (EXPR_OP):
        free_Expr_op(expr->op);
        break;
    }

    free(expr);
}

void print_Expr(struct Expr *expr) {
    switch (expr->type) {
    case (EXPR_CONSTANT):
        print_Constant(expr->constant->constant);
        break;
    case (EXPR_IDENTIFIER):
        print_Identifier(expr->id->id);
        break;
    case (EXPR_OP):
        print_Expr(expr->op->arg1);
        printf(" %d ", expr->op->op);
        print_Expr(expr->op->arg2);
        break;
    }
}

struct Statement_assignment *make_Assignment(struct Identifier *id, struct Expr *expr) {
    struct Statement_assignment  *assignment = malloc(sizeof(struct Statement_assignment));
    check_mem(assignment);

    assignment->id = id;
    assignment->expr = expr;
    return assignment;
}

void free_Statement_assignment(struct Statement_assignment *assignment) {
    free_Identifier(assignment->id);
    free_Expr(assignment->expr);
    free(assignment);
}

struct Statement_return *make_Return(struct Expr *expr) {
    struct Statement_return *ret = malloc(sizeof(struct Statement_return));
    check_mem(ret);

    ret->expr = expr;

    return ret;
}

void free_Statement_return(struct Statement_return *ret) {
    free_Expr(ret->expr);
    free(ret);
}

struct Statement *make_Statement(enum Statement_type type) {
    struct Statement *stmt = malloc(sizeof(struct Statement));
    check_mem(stmt);
    stmt->type = type;
    return stmt;
}

void free_Statement(struct Statement *stmt) {
    switch(stmt->type) {
    case STMT_ASSIGN:
        free_Statement_assignment(stmt->assignment);
        break;
    case STMT_RETURN:
        free_Statement_return(stmt->ret);
        break;
    }

    free(stmt);
}

void print_Statement(struct Statement *stmt) {
    switch(stmt->type) {
    case STMT_ASSIGN:
        printf("Assignment: %s = ", stmt->assignment->id->id);
        print_Expr(stmt->assignment->expr);
        break;
    case STMT_RETURN:
        printf("Return: ");
        print_Expr(stmt->ret->expr);
        break;
    }
    printf("\n");
}


struct Function *make_Function(struct Type *return_type, struct Identifier *name, GList *param_declarations, GList *body_declarations, GList *body_statements) {
    struct Function *function = malloc(sizeof(struct Function));
    check_mem(function);

    function->return_type = return_type;
    function->name = name;
    function->param_declarations = param_declarations;
    function->body_declarations = body_declarations;
    function->body_statements = body_statements;

    return function;
}

void free_Function(struct Function *func) {
    free_Type(func->return_type);
    free_Identifier(func->name);
    g_list_free_full(func->param_declarations, (GDestroyNotify)free_Declaration);
    g_list_free_full(func->body_declarations, (GDestroyNotify)free_Declaration);
    g_list_free_full(func->body_statements, (GDestroyNotify)free_Statement);
    free(func);
}

void print_Function(struct Function *func) {
    printf("Function:\nName: %s\nReturn Type: %s\nParams:", func->name->id, func->return_type->type);
    g_list_foreach(func->param_declarations, (GFunc)print_Declaration, NULL);
    printf("Body Declarations:\n");
    g_list_foreach(func->body_declarations, (GFunc)print_Declaration, NULL);
    printf("Body Statements:\n");
    g_list_foreach(func->body_statements, (GFunc)print_Statement, NULL);
}

struct Program *make_Program(GList *declarations, GList *functions) {
    struct Program *prog = malloc(sizeof(struct Program));
    check_mem(prog);

    prog->declarations = declarations;
    prog->functions = functions;
    return prog;
}

void free_Program(struct Program *prog) {
    g_list_free_full(prog->declarations, (GDestroyNotify)free_Declaration);
    g_list_free_full(prog->functions, (GDestroyNotify)free_Function);
    free(prog);
}

void print_Program(struct Program *prog) {
    printf("Program:\n");
    g_list_foreach(prog->declarations, (GFunc)print_Declaration, NULL);
    g_list_foreach(prog->functions, (GFunc)print_Function, NULL);
}
