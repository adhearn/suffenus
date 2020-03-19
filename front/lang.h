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

enum NodeType {
    NODE_TYPE_IDENTIFIER,
    NODE_TYPE_CONSTANT,
    NODE_TYPE_DECLARATION,
    NODE_TYPE_EXPR,
    NODE_TYPE_STATEMENT,
    NODE_TYPE_BLOCK,
    NODE_TYPE_FUNCTION,
    NODE_TYPE_PROGRAM,
};

enum StatementType {
    STMT_TYPE_COMPOUND,
    STMT_TYPE_EXPR,
    STMT_TYPE_JUMP,
    STMT_TYPE_RETURN,
    STMT_TYPE_SELECTION
};

struct Type {
    char *type;
};

struct Identifier {
    enum NodeType node_type;
    char *id;
    struct Type *type;
};

struct Constant {
    enum NodeType node_type;
    int val;
};

struct Declaration {
    enum NodeType node_type;
    struct Type *type;
    struct Identifier *id;
};

enum ExprType {
    EXPR_ASSIGNMENT,
    EXPR_BINOP,
    EXPR_CALL,
    EXPR_CONSTANT,
    EXPR_IDENTIFIER,
    EXPR_RELOP,
};

struct ExprAssignment {
    struct Identifier *lhs;
    struct Expr *rhs;
};

struct ExprCall {
    struct Expr *function;
    GList *args;
};

struct ExprConstant {
    struct Constant *constant;
};

struct ExprIdentifier {
    struct Identifier *id;
};

struct ExprOp {
    enum Op op;
    struct Expr *arg1;
    struct Expr *arg2;
};

struct Expr {
    enum NodeType node_type;
    enum ExprType type;
    union {
        struct ExprAssignment *assignment;
        struct ExprCall *call;
        struct ExprConstant *constant;
        struct ExprIdentifier *id;
        struct ExprOp *op;
    };
};


struct StatementSelection {
    struct Expr *test;
    struct Statement *conseq;
    struct Statement *alt;
};

enum StatementJumpType {
    JUMP_BREAK,
    JUMP_CONTINUE,
    JUMP_GOTO,
    JUMP_RETURN
};

struct StatementJump {
    enum StatementJumpType type;
    union {
        struct Identifier *identifier;
        struct Expr *expr;
    };
};

struct Statement {
    enum NodeType node_type;
    enum StatementType type;
    union {
        /* struct Statement_return *ret; */
        /* struct Statement_assignment *assignment; */
        struct Block *compound;
        struct Expr *expr;
        struct StatementJump *jump;
        struct StatementSelection *selection;
    };
};

struct Block {
    enum NodeType node_type;
    GList *block_elements;
    struct SymbolTable *st;
};

struct Function {
    enum NodeType node_type;
    struct Type *return_type;
    struct Identifier *name;
    GList *param_declarations;
    struct Block *body;
};

struct Program {
    enum NodeType node_type;
    struct Block *top_level_block;
};


enum NodeType ast_node_type(void *ast_node);
struct Type *type_new(char *type);
void type_make_fn_type(struct Type *type);

struct Identifier *identifier_new(char *id, struct Type *type);
void identifier_free(struct Identifier *identifier);

struct Constant *constant_new(int constant);
void constant_free(struct Constant *constant);

struct Declaration *declaration_new(struct Type *type, struct Identifier *id);
void declaration_free(struct Declaration *declaration);
// void declaration_print(struct Declaration *declaration);


struct ExprAssignment *expr_assignment_new(struct Identifier *lhs, struct Expr *rhs);
struct ExprCall *expr_call_new(struct Expr *function, GList *args);
struct ExprConstant *expr_constant_new(struct Constant *constant);
struct ExprIdentifier *expr_identifier_new(struct Identifier *id);
struct ExprOp *expr_op_new(enum Op op, struct Expr *arg1, struct Expr *arg2);
struct Expr *expr_new(enum ExprType type);
void expr_free(struct Expr *expr);

struct StatementJump *statement_jump_new();
struct StatementSelection *statement_selection_new();
void statement_jump_free();
void statement_selection_free();
struct Statement *statement_new(enum StatementType type);
void statement_free(struct Statement *statement);

struct Block *block_new(GList *block_elements, struct SymbolTable *st);
struct Block *block_extend(struct Block *block, void *elt);
void block_free(struct Block *block);
// void block_print(struct Block *block);
void block_element_free();
// void block_element_print();

struct Function *function_new(struct Type *return_type, struct Identifier *name, GList *param_declarations, struct Block *block);
void function_free(struct Function *function);
// void function_print(struct Function *function);

struct Program *program_new(struct Block *top_leveL_nodes);
void program_free(struct Program *prog);
// void program_print(struct Program *prog);


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
