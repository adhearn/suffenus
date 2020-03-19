#include <stdio.h>
#include <glib.h>
#include "util.h"
#include "lang.h"
#include "icg.h"

GList *quads;

void extend_quads(struct Quad *quad) {
    quads = g_list_append(quads, quad);
}

struct Address *label_new() {
    static int count = 0;
    char **label = malloc(sizeof(char *));
    check_mem(label);
    asprintf(label, "L%d", count);
    count++;

    struct Address *addr = address_new(ADDR_TYPE_LABEL);
    addr->name = *label;
    return addr;
}

struct Address *address_new(enum TACAddressType type) {
    struct Address *addr = malloc(sizeof(struct Address));
    check_mem(addr);
    addr->type = type;
    return addr;
}

struct Address *address_temp_new() {
    static int count = 0;
    struct Address *addr = address_new(ADDR_TYPE_TEMP);
    asprintf(&addr->name, "TMP%d", count);
    count++;
    return addr;
}

struct Address *address_zero_new() {
    struct Address *addr = address_new(ADDR_TYPE_CONSTANT);
    addr->value = 0;
    return addr;
}

void address_print(struct Address *addr) {
    switch(addr->type) {
    case (ADDR_TYPE_LABEL):
    case (ADDR_TYPE_NAME):
    case (ADDR_TYPE_TEMP):
        printf("%s", addr->name);
        break;
    case (ADDR_TYPE_CONSTANT):
        printf("%d", addr->value);
        break;
    default:
        log_err("Invalid address type in address_print: %d\n", addr->type);
    }
}

void label_print(struct Address *label) {
    printf("%s: ", label->name);
}

void quad_add_label(struct Quad *quad, struct Address *label) {
    quad->labels = g_list_append(quad->labels, label);
}

struct Quad *quad_new(enum TACInstruction instruction) {
    struct Quad *quad = malloc(sizeof(struct Quad));
    check_mem(quad);
    quad->instruction = instruction;
    return quad;
}

void quad_print(struct Quad *quad) {
    g_list_foreach(quad->labels, (gpointer)label_print, NULL);
    switch (quad->instruction) {
    case (TAC_NO_OP):
        printf("<NO OP>");
        break;
    case (TAC_UNOP):
        address_print(quad->result);
        printf(" = ");
        printf(" %s ", OP_NAMES[quad->op]);
        address_print(quad->arg1);
        break;
    case (TAC_BINOP):
        address_print(quad->result);
        printf(" = ");
        address_print(quad->arg1);
        printf(" %s ", OP_NAMES[quad->op]);
        address_print(quad->arg2);
        break;
    case (TAC_COPY):
        address_print(quad->result);
        printf(" = ");
        address_print(quad->arg1);
        break;
    case (TAC_JUMP):
        printf("JUMP ");
        address_print(quad->result);
        break;
    case (TAC_RETURN):
        printf("RETURN ");
        address_print(quad->result);
        break;
    case (TAC_CONDITIONAL):
        printf("IF ");
        address_print(quad->arg1);
        printf(" %s ", OP_NAMES[quad->op]);
        address_print(quad->arg2);
        printf(" JUMP ");
        address_print(quad->result);
        break;
    }
    printf("\n");
}

struct Quad *expr_assignment_generate_tac_quads_rvalue(struct ExprAssignment *assignment) {
    struct Quad *final_expr = expr_generate_tac_quads_rvalue(assignment->rhs);
    struct Address *addr = address_new(ADDR_TYPE_NAME);
    addr->name = assignment->lhs->id;

    if (final_expr->instruction == TAC_COPY && final_expr->result->type == ADDR_TYPE_TEMP) {
        // Small optimization: if the final expr from above is a copy to a temp, we can instead
        // copy to the variable we're trying to assign to anyway. Because it was a temp,
        // nothing else should be relying on its name (i.e. there should be no occurrences)
        // of the temp after this point in the program), so we're safe to eliminate it.
        final_expr->result = addr;
        return final_expr;
    } else {
        struct Quad *assignment_quad = quad_new(TAC_COPY);
        assignment_quad->result = addr;
        assignment_quad->arg1 = final_expr->result;
        extend_quads(assignment_quad);
        return assignment_quad;
    }
}


struct Quad *expr_constant_generate_tac_quads_rvalue(struct ExprConstant* constant) {
    struct Address *rhs = address_new(ADDR_TYPE_CONSTANT);
    rhs->value = constant->constant->val;
    struct Address *lhs = address_temp_new();
    struct Quad *quad = quad_new(TAC_COPY);
    quad->result = lhs;
    quad->arg1 = rhs;
    extend_quads(quad);
    return quad;
}

struct Quad *expr_identifier_generate_tac_quads_rvalue(struct ExprIdentifier *id) {
    struct Address *rhs = address_new(ADDR_TYPE_NAME);
    rhs->name = id->id->id;
    struct Address *lhs = address_temp_new();
    struct Quad *quad = quad_new(TAC_COPY);
    quad->result = lhs;
    quad->arg1 = rhs;
    extend_quads(quad);
    return quad;
}

struct Quad *expr_op_generate_tac_quads_rvalue(struct ExprOp *op) {
    struct Quad *quad1 = expr_generate_tac_quads_rvalue(op->arg1);
    struct Quad *quad2 = expr_generate_tac_quads_rvalue(op->arg2);
    // We could try to optimize this here if these are both copies. However, a later optimization
    // pass can also handle it. For now, keep things simple.
    struct Address *result_addr = address_temp_new();
    struct Quad *quad = quad_new(TAC_BINOP);
    quad->arg1 = quad1->result;
    quad->arg2 = quad2->result;
    quad->result = result_addr;
    quad->op = op->op;
    extend_quads(quad);
    return quad;
}

struct Quad *expr_generate_tac_quads_rvalue(struct Expr *expr) {
    struct Quad *quad;
    switch (expr->type) {
    case (EXPR_ASSIGNMENT):
        quad = expr_assignment_generate_tac_quads_rvalue(expr->assignment);
        break;
    case (EXPR_CONSTANT):
        quad = expr_constant_generate_tac_quads_rvalue(expr->constant);
        break;
    case (EXPR_IDENTIFIER):
        quad = expr_identifier_generate_tac_quads_rvalue(expr->id);
        break;
    case (EXPR_BINOP):
    case (EXPR_RELOP):
        quad = expr_op_generate_tac_quads_rvalue(expr->op);
        break;
    }
    return quad;
}

struct Quad *expr_constant_generate_tac_quads_jump(struct ExprConstant *constant, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Quad *jump = quad_new(TAC_JUMP);
    if (constant->constant->val) {
        jump->result = true_label;
    } else {
        jump->result = false_label;
    }
    extend_quads(jump);
    return jump;
}

// Helper function when generate the TAC quads for a conditional over a name (i.e. a temp or a variable)
struct Quad *address_generate_conditional_jump(struct Address *addr, struct Address *true_label, struct Address *false_label) {
    struct Quad *quad_true = quad_new(TAC_CONDITIONAL);
    quad_true->op = Op_neq;
    quad_true->arg1 = addr;
    quad_true->arg2 = address_zero_new();
    struct Quad *quad_false = quad_new(TAC_JUMP);
    quad_false->result = false_label;
    extend_quads(quad_true);
    extend_quads(quad_false);
    return quad_false;
}

struct Quad *expr_assignment_generate_tac_quads_jump(struct ExprAssignment *assignment, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Quad *assignment_quad = expr_assignment_generate_tac_quads_rvalue(assignment);
    struct Address *addr = assignment_quad->result;
    return address_generate_conditional_jump(addr, true_label, false_label);
}

struct Quad *expr_identifier_generate_tac_quads_jump(struct ExprIdentifier *identifier, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Address *identifier_address = address_new(ADDR_TYPE_NAME);
    identifier_address->name = identifier->id->id;
    return address_generate_conditional_jump(identifier_address, true_label, false_label);
}

struct Quad *expr_binop_generate_tac_quads_jump(struct ExprOp *op, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Quad *binop_quad = expr_op_generate_tac_quads_rvalue(op);
    struct Address *binop_result = binop_quad->result;
    return address_generate_conditional_jump(binop_result, true_label, false_label);
}

struct Quad *expr_relop_generate_tac_quads_jump(struct ExprOp *op, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Quad *arg1_quad = expr_generate_tac_quads_rvalue(op->arg1);
    struct Quad *arg2_quad = expr_generate_tac_quads_rvalue(op->arg2);
    struct Quad *relop_quad = quad_new(TAC_CONDITIONAL);
    relop_quad->arg1 = arg1_quad->result;
    relop_quad->arg2 = arg2_quad->result;
    relop_quad->op = op->op;
    relop_quad->result = true_label;
    struct Quad *false_quad = quad_new(TAC_JUMP);
    false_quad->result = false_label;
    extend_quads(relop_quad);
    extend_quads(false_quad);
    return false_quad;
}

struct Quad *expr_generate_tac_quads_jump(struct Expr *expr, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Quad *quad;
    switch (expr->type) {
    case (EXPR_ASSIGNMENT):
        quad = expr_assignment_generate_tac_quads_jump(expr->assignment, true_label, false_label, next_label);
        break;
    case (EXPR_CONSTANT):
        quad = expr_constant_generate_tac_quads_jump(expr->constant, true_label, false_label, next_label);
        break;
    case (EXPR_IDENTIFIER):
        quad = expr_identifier_generate_tac_quads_jump(expr->id, true_label, false_label, next_label);
        break;
    case (EXPR_BINOP):
        quad = expr_binop_generate_tac_quads_jump(expr->op, true_label, false_label, next_label);
        break;
    case (EXPR_RELOP):
        quad = expr_relop_generate_tac_quads_jump(expr->op, true_label, false_label, next_label);
        break;
    }
    return quad;
}

struct Quad *declaration_generate_tac_quads(struct Declaration *decl) {
    struct Quad *noop = quad_new(TAC_NO_OP);
    extend_quads(noop);
    return noop;
}

struct Quad *statement_jump_generate_tac_quads(struct StatementJump *jump) {
    struct Quad *quad;
    switch (jump->type) {
    case JUMP_BREAK:
    case JUMP_CONTINUE:
    case JUMP_GOTO:
        break;
    case JUMP_RETURN: {
        struct Quad *quad_expr = expr_generate_tac_quads_rvalue(jump->expr);
        struct Quad *quad_return = quad_new(TAC_RETURN);
        quad_return->result = quad_expr->result;
        extend_quads(quad_return);
        quad = quad_return;
    }
    }
    return quad;
}

struct Quad *statement_selection_generate_tac_quads(struct StatementSelection* selection, struct Address *rejoin_label) {
    /**
     * One of the more complicated cases. The gist is that we have to generate 2 possible labels:
     * one for the true case, one for the false case. The third label will  point to where the branches
     * rejoin, and is passed in as an arg. (Because this case is recursive, we can't generate it here,
     * it has to be passed in.) NOTE: as currently designed, this generates a ton of extra No-Ops. This
     * is a prime spot for optimization.
     */
    struct Quad *quad;
    struct Address *true_label = label_new();
    struct Quad *true_noop = quad_new(TAC_NO_OP);
    quad_add_label(true_noop, true_label);

    struct Address *false_label;

    if (selection->alt) {
        false_label = label_new();
    } else {
        false_label = rejoin_label;
    }

    expr_generate_tac_quads_jump(selection->test, true_label, false_label, rejoin_label);
    extend_quads(true_noop);
    quad = statement_generate_tac_quads(selection->conseq, rejoin_label);

    if (selection->alt) {
        struct Quad *false_noop = quad_new(TAC_NO_OP);
        quad_add_label(false_noop, false_label);
        extend_quads(false_noop);
        quad = statement_generate_tac_quads(selection->alt, rejoin_label);
    }
    return quad;
}

struct Quad *statement_generate_tac_quads(struct Statement* stmt, struct Address *rejoin_label) {
    struct Quad *quad;
    switch(stmt->type) {
    case (STMT_TYPE_COMPOUND):
        quad = block_generate_tac_quads(stmt->compound);
        break;
    case (STMT_TYPE_EXPR):
        quad = expr_generate_tac_quads_rvalue(stmt->expr);
        break;
    case (STMT_TYPE_JUMP):
        quad = statement_jump_generate_tac_quads(stmt->jump);
        break;
    case (STMT_TYPE_RETURN):
        quad = expr_generate_tac_quads_rvalue(stmt->expr);
        break;
    case (STMT_TYPE_SELECTION):
        quad = statement_selection_generate_tac_quads(stmt->selection, rejoin_label);
        break;
    }
    return quad;
}

struct Quad *function_generate_tac_quads(struct Function *func) {
    struct Address *label = label_new();
    struct Quad *function_start_noop = quad_new(TAC_NO_OP);
    quad_add_label(function_start_noop, label);
    extend_quads(function_start_noop);

    // TODO: generate quads for function params

    block_generate_tac_quads(func->body);

    GList *last = g_list_last(quads);
    if (last) {
        return last->data;
    } else {
        return NULL;
    }
}


struct Quad *block_element_generate_tac_quads(void *elt) {
    struct Quad *quad;
    enum NodeType node_type = ast_node_type(elt);
    switch (node_type) {
    case NODE_TYPE_FUNCTION:
        quad = function_generate_tac_quads((struct Function *)elt);
        break;
    case NODE_TYPE_DECLARATION:
        // quad = declaration_generate_tac_quads((struct Declaration *)elt);
        break;
    case NODE_TYPE_STATEMENT: {
        struct Address *rejoin_label = label_new();
        struct Quad *rejoin_quad = quad_new(TAC_NO_OP);
        quad_add_label(rejoin_quad, rejoin_label);
        quad = statement_generate_tac_quads((struct Statement*) elt, rejoin_label);
        extend_quads(rejoin_quad);
        quad = rejoin_quad;
        break;
    }
    default:
        log_err("Received invalid node type in block_element_free: %d", node_type);
    }
    return quad;
}

struct Quad *block_generate_tac_quads(struct Block *block) {
    g_list_foreach(block->block_elements, (gpointer)block_element_generate_tac_quads, NULL);
    GList *last = g_list_last(quads);
    if (last) {
        return last->data;
    } else {
        return NULL;
    }
}

GList *program_generate_tac_quads(struct Program *prog) {
    block_generate_tac_quads(prog->top_level_block);
    return quads;
}
