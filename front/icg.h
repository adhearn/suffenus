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
    // TAC_CODITIONAL_NO_OP,
    TAC_CONDITIONAL,
    // TAC_PARAM,
    // TAC_CALL,
    TAC_RETURN,
    // TAC_INDEXED_COPY,
};

enum TACAddressType {
    ADDR_TYPE_NAME,
    ADDR_TYPE_CONSTANT,
    ADDR_TYPE_TEMP,
    ADDR_TYPE_LABEL,
    // ADDR_TYPE_FUNC_PARAM
};

struct Address {
    enum TACAddressType type;
    union {
        int value;
        char *name;
    };
};

struct Quad {
    enum TACInstruction instruction;
    GList *labels;
    struct Address *arg1;
    struct Address *arg2;
    struct Address *result;
    enum Op op;
};

void extend_quads(struct Quad *quad);
void quad_add_label(struct Quad *quad, struct Address *label);
struct Quad *quad_new(enum TACInstruction instruction);
struct Address *label_new();
struct Address *address_new(enum TACAddressType type);
struct Address *address_temp_new();
void address_print(struct Address *addr);
void quad_print(struct Quad *quad);

struct Quad *expr_constant_generate_tac_quads_rvalue(struct ExprConstant* constant);
struct Quad *expr_identifier_generate_tac_quads_rvalue(struct ExprIdentifier *id);
struct Quad *expr_op_generate_tac_quads_rvalue(struct ExprOp *op);
struct Quad *expr_generate_tac_quads_rvalue(struct Expr *expr);
struct Quad *expr_constant_generate_tac_quads_jump(struct ExprConstant* constant, struct Address *true_label, struct Address *false_label, struct Address *next_label);
struct Quad *expr_identifier_generate_tac_quads_jump(struct ExprIdentifier *id, struct Address *true_label, struct Address *false_label, struct Address *next_label);
struct Quad *expr_binop_generate_tac_quads_jump(struct ExprOp *op, struct Address *true_label, struct Address *false_label, struct Address *next_label);
struct Quad *expr_relop_generate_tac_quads_jump(struct ExprOp *op, struct Address *true_label, struct Address *false_label, struct Address *next_label);
struct Quad *expr_generate_tac_quads_jump(struct Expr *expr, struct Address *true_label, struct Address *false_label, struct Address *next_label);

struct Quad *declaration_generate_tac_quads(struct Declaration *elt);

struct Quad *statement_selection_generate_tac_quads(struct StatementSelection *selection, struct Address *rejoin_label);
struct Quad *statement_generate_tac_quads(struct Statement* elt, struct Address *rejoin_label);

struct Quad *function_generate_tac_quads(struct Function *elt);
struct Quad *block_element_generate_tac_quads(void *elt);
struct Quad *block_generate_tac_quads(struct Block *block);
GList *program_generate_tac_quads(struct Program *prog);

#endif
