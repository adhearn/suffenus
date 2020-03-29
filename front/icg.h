#ifndef _icg_h
#define _icg_h

#include <glib.h>
#include "lang.h"

enum TACInstruction {
    TAC_NO_OP,
    TAC_UNOP,
    TAC_BINOP,
    TAC_COPY,
    TAC_JUMP,
    TAC_IF,
    TAC_CASE,
    TAC_PARAM,
    TAC_CALL,
    TAC_RETURN,
    TAC_COPY_INDEXED_LHS,
    TAC_COPY_INDEXED_RHS,
};

enum TACIndexedCopyType {
    INDEXED_LVAL,
    INDEXED_RVAL,
};

enum TACAddressType {
    ADDR_TYPE_NAME,
    ADDR_TYPE_CONSTANT,
    ADDR_TYPE_TEMP,
    ADDR_TYPE_LABEL,
    ADDR_TYPE_FUNC_PARAM
};

struct Address {
    enum TACAddressType type;
    union {
        int value; // The value for a constant, or the param position for params
        char *name; // The label or variable name for labels, names, and temps
    };
    GList *prefixes;
};

struct Quad {
    enum TACInstruction instruction;
    GList *labels;
    struct Address *arg1;
    struct Address *arg2;
    struct Address *result;
    union {
        enum Op op;
        enum TACIndexedCopyType indexed_copy_type;
    };
};

struct SwitchQueuePair {
    struct Expr *test;
    struct Address *label;
};

void extend_quads(struct Quad *quad);
void quad_add_label(struct Quad *quad, struct Address *label);
struct Quad *quad_new(enum TACInstruction instruction);
struct Quad *quad_labeled_noop_new(struct Address *label);
struct Address *label_new();
struct Address *label_from_identifier();
struct Address *address_new(enum TACAddressType type);
struct Address *address_temp_new();
struct Address *address_from_identifier(struct Identifier *identifier);
void address_print(struct Address *addr);
void quad_print(struct Quad *quad);

struct Quad *expr_constant_generate_tac_quads_rvalue(struct ExprConstant* constant);
struct Quad *expr_identifier_generate_tac_quads_rvalue(struct ExprIdentifier *id);
struct Quad *expr_indexed_generate_tac_quads_rvalue(struct ExprIndexed *indexed);
struct Quad *expr_op_generate_tac_quads_rvalue(struct ExprOp *op);
struct Quad *expr_generate_tac_quads_rvalue(struct Expr *expr);
struct Quad *expr_constant_generate_tac_quads_jump(struct ExprConstant* constant, struct Address *true_label, struct Address *false_label, struct Address *next_label);
struct Quad *expr_identifier_generate_tac_quads_jump(struct ExprIdentifier *id, struct Address *true_label, struct Address *false_label, struct Address *next_label);
struct Quad *expr_binop_generate_tac_quads_jump(struct ExprOp *op, struct Address *true_label, struct Address *false_label, struct Address *next_label);
struct Quad *expr_relop_generate_tac_quads_jump(struct ExprOp *op, struct Address *true_label, struct Address *false_label, struct Address *next_label);
struct Quad *expr_generate_tac_quads_jump(struct Expr *expr, struct Address *true_label, struct Address *false_label, struct Address *next_label);

struct Quad *declaration_generate_tac_quads(struct Declaration *elt);

struct Quad *statement_for_generate_tac_quads(struct StatementFor *loop, struct Address *rejoin_label, struct Address *outer_loop_start, struct Address *outer_loop_end);
struct Quad *statement_selection_generate_tac_quads(struct StatementSelection *selection, struct Address *rejoin_label, struct Address *loop_start, struct Address *loop_end);
struct Quad *statement_while_generate_tac_quads(struct StatementWhile *loop, struct Address *rejoin_label, struct Address *outer_loop_start, struct Address *outer_loop_end);
struct Quad *statement_generate_tac_quads(struct Statement* elt, struct Address *rejoin_label, struct Address *loop_start, struct Address *loop_end);

struct Quad *function_generate_tac_quads(struct Function *elt);
struct Quad *block_element_generate_tac_quads(void *elt, struct Address *loop_start, struct Address *loop_end);
struct Quad *block_generate_tac_quads(struct Block *block, struct Address *loop_start, struct Address *loop_end);
GList *program_generate_tac_quads(struct Program *prog);

#endif
