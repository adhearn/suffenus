#ifndef _lang_h
#define _lang_h

#include <glib.h>

enum Op {
    /* Op_null, */
    /* Op_or, */
    /* Op_and, */
    Op_add,
    Op_sub,
    Op_mul,
    Op_div,
    /* Op_mod, */
    /* Op_lshift, */
    /* Op_rshift, */
    Op_eq,
    Op_neq,
    Op_lt,
    Op_gt,
    Op_lte,
    Op_gte,
};

enum Expr_type {
    EXPR_OP,
    EXPR_CONSTANT,
    EXPR_IDENTIFIER
};

enum Statement_type {
    STMT_RETURN,
    STMT_ASSIGN
};

struct Type {
    char *type;
};

struct Identifier {
    char *id;
    struct Type *type;
};

struct Constant {
    int val;
};

struct Declaration {
    struct Type *type;
    struct Identifier *id;
};

struct Expr_op {
    enum Op op;
    struct Expr *arg1;
    struct Expr *arg2;
};

struct Expr_identifier {
    struct Identifier *id;
};

struct Expr_constant {
    struct Constant *constant;
};

struct Expr {
    enum Expr_type type;
    union {
        struct Expr_op *op;
        struct Expr_identifier *id;
        struct Expr_constant *constant;
    };
};

struct Statement_return {
    struct Expr *expr;
};

struct Statement_assignment {
    struct Identifier *id;
    struct Expr *expr;
};


struct Statement {
    enum Statement_type type;
    union {
        struct Statement_return *ret;
        struct Statement_assignment *assignment;
    };
};

struct Function {
    struct Type *return_type;
    struct Identifier *name;
    GList *param_declarations;
    GList *body_declarations;
    GList *body_statements;
};

struct Program {
    GList *declarations;
    GList *functions;
};


struct Type *make_Type(char *type);
struct Identifier *make_Identifier(char *id);
struct Constant *make_Constant(int constant);
struct Declaration *make_Declaration(struct Type *type, struct Identifier *id);
struct Statement_return *make_Return(struct Expr *expr);
struct Statement_assignment *make_Assignment(struct Identifier *id, struct Expr *expr);
struct Statement *make_Statement(enum Statement_type type);
struct Expr_op *make_Expr_Op(enum Op op, struct Expr *arg1, struct Expr *arg2);
struct Expr_identifier *make_Expr_Identifier(struct Identifier *id);
struct Expr_constant *make_Expr_Constant(struct Constant *constant);
struct Expr *make_Expr(enum Expr_type type);
struct Function *make_Function(struct Type *return_type, struct Identifier *name, GList *param_declarations, GList *body_declarations, GList *body_statements);
struct Program *make_Program(GList *declarations, GList *functions);
void free_Declaration(struct Declaration *declaration);
void free_Expr(struct Expr *expr);
void free_Statement(struct Statement *statement);
void free_Function(struct Function *function);
void free_Program(struct Program *prog);
void print_Program(struct Program *prog);
#endif
