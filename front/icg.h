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
    // TAC_CONDITIONAL_RELOP,
    // TAC_PARAM,
    // TAC_CALL,
    TAC_RETURN,
    // TAC_INDEXED_COPY,
};

enum TACAddressType {
    ADDR_TYPE_NAME,
    ADDR_TYPE_CONSTANT,
    ADDR_TYPE_TEMP,
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
void quad_add_label(struct Quad *quad, char *label);
struct Quad *quad_new(enum TACInstruction instruction);
void label_print(char *label);
void address_print(struct Address *addr);
void quad_print(struct Quad *quad);
struct Quad *expr_constant_generate_tac_quads(struct Expr_constant* constant);
struct Quad *expr_identifier_generate_tac_quads(struct Expr_identifier *id);
struct Quad *expr_op_generate_tac_quads(struct Expr_op *op);
struct Quad *expr_generate_tac_quads(struct Expr *expr);
struct Quad *declaration_generate_tac_quads(struct Declaration *elt);
struct Quad *statement_assignment_generate_tac_quads(struct Statement_assignment *assignment);
struct Quad *statement_return_generate_tac_quads(struct Statement_return *ret);
struct Quad *statement_generate_tac_quads(struct Statement* elt);
struct Quad *function_generate_tac_quads(struct Function *elt);
struct Quad *block_element_generate_tac_quads(void *elt);
void block_generate_tac_quads(struct Block *block);
GList *program_generate_tac_quads(struct Program *prog);

#endif
