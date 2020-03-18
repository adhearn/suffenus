#ifndef _lang_h
#define _lang_h

#include <glib.h>

enum Op {
    Op_add,
    Op_sub,
    Op_mul,
    Op_div,
    Op_eq,
    Op_neq,
    Op_lt,
    Op_gt,
    Op_lte,
    Op_gte,
    /* Op_null, */
    /* Op_or, */
    /* Op_and, */
    /* Op_mod, */
    /* Op_lshift, */
    /* Op_rshift, */
};

// Bigger than necessary, but good enough
char *OP_NAMES[30];

enum Node_type {
    NODE_TYPE_IDENTIFIER,
    NODE_TYPE_CONSTANT,
    NODE_TYPE_DECLARATION,
    NODE_TYPE_EXPR,
    NODE_TYPE_STATEMENT,
    NODE_TYPE_BLOCK,
    NODE_TYPE_FUNCTION,
    NODE_TYPE_PROGRAM,
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
    enum Node_type node_type;
    char *id;
    struct Type *type;
};

struct Constant {
    enum Node_type node_type;
    int val;
};

struct Declaration {
    enum Node_type node_type;
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
    enum Node_type node_type;
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
    enum Node_type node_type;
    enum Statement_type type;
    union {
        struct Statement_return *ret;
        struct Statement_assignment *assignment;
    };
};

struct Block {
    enum Node_type node_type;
    GList *block_elements;
    struct SymbolTable *st;
};

struct Function {
    enum Node_type node_type;
    struct Type *return_type;
    struct Identifier *name;
    GList *param_declarations;
    struct Block *body;
};

struct Program {
    enum Node_type node_type;
    struct Block *top_level_block;
};


enum Node_type ast_node_type(void *ast_node);
struct Type *type_new(char *type);
void type_make_fn_type(struct Type *type);
struct Identifier *identifier_new(char *id, struct Type *type);
struct Constant *constant_new(int constant);
struct Declaration *declaration_new(struct Type *type, struct Identifier *id);
struct Statement_return *return_new(struct Expr *expr);
struct Statement_assignment *assignment_new(struct Identifier *id, struct Expr *expr);
struct Statement *statement_new(enum Statement_type type);
struct Expr_op *expr_op_new(enum Op op, struct Expr *arg1, struct Expr *arg2);
struct Expr_identifier *expr_identifier_new(struct Identifier *id);
struct Expr_constant *expr_constant_new(struct Constant *constant);
struct Expr *expr_new(enum Expr_type type);
struct Block *block_new(GList *block_elements, struct SymbolTable *st);
struct Block *block_extend(struct Block *block, void *elt);
struct Function *function_new(struct Type *return_type, struct Identifier *name, GList *param_declarations, struct Block *block);
struct Program *program_new(struct Block *top_leveL_nodes);
void identifier_free(struct Identifier *identifier);
void declaration_free(struct Declaration *declaration);
void declaration_print(struct Declaration *declaration);
void expr_free(struct Expr *expr);
void statement_free(struct Statement *statement);
void block_element_free();
void block_element_print();
void block_free(struct Block *block);
void block_free(struct Block *block);
void block_print(struct Block *block);
void function_free(struct Function *function);
void function_print(struct Function *function);
void program_free(struct Program *prog);
void program_print(struct Program *prog);

struct SymbolTable {
    struct SymbolTable *parent;
    GHashTable *table;
};

struct SymbolTable *symbol_table_new(struct SymbolTable* parent);
void symbol_table_free(struct SymbolTable *st);
void symbol_table_extend(struct SymbolTable *st, struct Identifier *id);
struct Identifier *symbol_table_lookup(struct SymbolTable *st, char *key);
struct Identifier *symbol_table_lookup_local(struct SymbolTable *st, char *key);

#endif
