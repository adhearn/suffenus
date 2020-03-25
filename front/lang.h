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

enum StorageClassSpecifier {
    SPEC_TYPEDEF,
    SPEC_EXTERN,
    SPEC_STATIC,
    SPEC_AUTO,
    SPEC_REGISTER,
};

enum TypeSpecifier {
    TYPE_VOID,
    TYPE_CHAR,
    TYPE_SHORT,
    TYPE_INT,
    TYPE_LONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_SIGNED,
    TYPE_UNSIGNED,
    TYPE_BOOL,
    TYPE_COMPLEX,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_ENUM,
    TYPE_TYPEDEF
};

struct StructUnionSpecifier {
    enum TypeSpecifier struct_or_union; // Should only be TYPE_STRUCT or TYPE_UNION
    GList *declarations;
};

struct EnumSpecifier {
    GList *enumerators;
};

enum TypeQualifier {
    QUALIFIER_CONSTANT,
    QUALIFIER_RESTRICT,
    QUALIFIER_VOLATILE,
};

enum DeclaratorType {
    DECLARATOR_IDENTIFIER,
    DECLARATOR_ARRAY,
    DECLARATOR_FUNCTION,
};

struct Declarator {
    enum DeclaratorType type;
    struct Declarator *sub_declarator;
    struct Expr *initializer;
    union {
        struct Identifier *identifier;
        GList *param_or_identifier_list;
        struct Expr *array_size_expr;
    };
};

struct Declaration {
    enum NodeType node_type;
    GList *specifiers;
    GList *declarators;
};

enum ExprType {
    EXPR_ASSIGNMENT,
    EXPR_BINOP,
    EXPR_CALL,
    EXPR_CONSTANT,
    EXPR_IDENTIFIER,
    EXPR_INDEXED,
    EXPR_RELOP,
};

enum LValueType {
    LVALUE_IDENTIFIER,
    LVALUE_INDEXED,
};

struct ExprAssignment {
    enum LValueType lvalue_type;
    void *lhs; // Should be either an identifier or an ExprIndexed
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

struct ExprIndexed {
    struct Expr *expr;
    struct Expr *index;
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
        struct ExprIndexed *indexed;
        struct ExprOp *op;
    };
};

enum StatementType {
    STMT_TYPE_COMPOUND,
    STMT_TYPE_EXPR,
    STMT_TYPE_FOR,
    STMT_TYPE_JUMP,
    STMT_TYPE_LABELED,
    STMT_TYPE_RETURN,
    STMT_TYPE_SELECTION,
    STMT_TYPE_SWITCH,
    STMT_TYPE_WHILE,
};

struct StatementFor {
    struct Expr *init;
    struct Expr *test;
    struct Expr *update;
    struct Statement *body;
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

enum StatementLabeledType {
    LABELED_CASE,
    LABELED_DEFAULT,
    LABELED_LABEL,
};

struct StatementLabeled {
    enum StatementLabeledType type;
    union {
        struct Identifier *label;
        struct Expr *test;
    };
    struct Statement *statement;
};

struct StatementSelection {
    struct Expr *test;
    struct Statement *conseq;
    struct Statement *alt;
};

struct StatementSwitch {
    struct Expr *test;
    struct Statement *body;
};

struct StatementWhile {
    struct Expr *test;
    struct Statement *body;
};

struct Statement {
    enum NodeType node_type;
    enum StatementType type;
    union {
        /* struct Statement_return *ret; */
        /* struct Statement_assignment *assignment; */
        struct Block *compound;
        struct Expr *expr;
        struct StatementFor *for_loop;
        struct StatementJump *jump;
        struct StatementLabeled *labeled;
        struct StatementSelection *selection;
        struct StatementSwitch *switch_statement;
        struct StatementWhile *while_loop;
    };
};

struct Block {
    enum NodeType node_type;
    GList *block_elements;
    struct SymbolTable *st;
};

struct Function {
    enum NodeType node_type;
    struct Declarator *declarator;
    struct Block *body;
    GList *specifiers;
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

struct Declarator *declarator_new(enum DeclaratorType type, struct Declarator *sub_declarator, void *other);
struct Identifier *declarator_identifier(struct Declarator *declarator);
void declarator_free(struct Declarator *declarator);

struct Declaration *declaration_new(GList *specifiers, GList *declarators);
void declaration_free(struct Declaration *declaration);
// void declaration_print(struct Declaration *declaration);


struct ExprAssignment *expr_assignment_new(enum LValueType lvalue_type, void *lhs, struct Expr *rhs);
struct ExprCall *expr_call_new(struct Expr *function, GList *args);
struct ExprConstant *expr_constant_new(struct Constant *constant);
struct ExprIdentifier *expr_identifier_new(struct Identifier *id);
struct ExprIndexed *expr_indexed_new(struct Expr *expr, struct Expr *index);
struct ExprOp *expr_op_new(enum Op op, struct Expr *arg1, struct Expr *arg2);
struct Expr *expr_new(enum ExprType type);
void expr_indexed_free(struct ExprIndexed *indexed);
void expr_free(struct Expr *expr);

struct StatementFor *statement_for_new(struct Expr *init, struct Expr *test, struct Expr *update, struct Statement *body);
struct StatementJump *statement_jump_new(enum StatementJumpType type);
struct StatementLabeled *statement_labeled_new(enum StatementLabeledType type, void *label_or_test, struct Statement *statement);
struct StatementSelection *statement_selection_new(struct Expr *test, struct Statement *conseq, struct Statement *alt);
struct StatementSwitch *statement_switch_new(struct Expr *test, struct Statement *body);
struct StatementWhile *statement_while_new(struct Expr *test, struct Statement *body);
void statement_for_free();
void statement_jump_free();
void statement_selection_free();
void statement_while_free();
struct Statement *statement_new(enum StatementType type);
void statement_free(struct Statement *statement);

struct Block *block_new(GList *block_elements, struct SymbolTable *st);
struct Block *block_extend(struct Block *block, void *elt);
void block_free(struct Block *block);
void block_element_free();

struct Function *function_new(struct Declarator *declarator, struct Block *block, GList *specifiers);
struct Identifier *function_identifier(struct Function *function);
void function_free(struct Function *function);

struct Program *program_new(struct Block *top_leveL_nodes);
void program_free(struct Program *prog);


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
