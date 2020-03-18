#include <stdio.h>
#include <glib.h>
#include "util.h"
#include "lang.h"
#include "icg.h"

GList *quads;

void extend_quads(struct Quad *quad) {
    quads = g_list_append(quads, quad);
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

void address_print(struct Address *addr) {
    switch(addr->type) {
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

void quad_add_label(struct Quad *quad, char *label) {
    quad->labels = g_list_append(quad->labels, label);
}

struct Quad *quad_new(enum TACInstruction instruction) {
    struct Quad *quad = malloc(sizeof(struct Quad));
    check_mem(quad);
    quad->instruction = instruction;
    return quad;
}

void label_print(char *label) {
    printf("%s: ", label);
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
    }
    printf("\n");
}

struct Quad *expr_constant_generate_tac_quads(struct Expr_constant* constant) {
    struct Address *rhs = address_new(ADDR_TYPE_CONSTANT);
    rhs->value = constant->constant->val;
    struct Address *lhs = address_temp_new();
    struct Quad *quad = quad_new(TAC_COPY);
    quad->result = lhs;
    quad->arg1 = rhs;
    extend_quads(quad);
    return quad;
}

struct Quad *expr_identifier_generate_tac_quads(struct Expr_identifier *id) {
    struct Address *rhs = address_new(ADDR_TYPE_NAME);
    rhs->name = id->id->id;
    struct Address *lhs = address_temp_new();
    struct Quad *quad = quad_new(TAC_COPY);
    quad->result = lhs;
    quad->arg1 = rhs;
    extend_quads(quad);
    return quad;
}

struct Quad *expr_op_generate_tac_quads(struct Expr_op *op) {
    struct Quad *quad1 = expr_generate_tac_quads(op->arg1);
    struct Quad *quad2 = expr_generate_tac_quads(op->arg2);
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

struct Quad *expr_generate_tac_quads(struct Expr *expr) {
    struct Quad *quad;
    switch (expr->type) {
    case (EXPR_CONSTANT):
        quad = expr_constant_generate_tac_quads(expr->constant);
        break;
    case (EXPR_IDENTIFIER):
        quad = expr_identifier_generate_tac_quads(expr->id);
        break;
    case (EXPR_OP):
        quad = expr_op_generate_tac_quads(expr->op);
        break;
    }
    return quad;
}

struct Quad *declaration_generate_tac_quads(struct Declaration *decl) {
    struct Quad *noop = quad_new(TAC_NO_OP);
    extend_quads(noop);
    return noop;
}

struct Quad *statement_assignment_generate_tac_quads(struct Statement_assignment *assignment) {
    struct Quad *final_expr = expr_generate_tac_quads(assignment->expr);
    struct Address *addr = address_new(ADDR_TYPE_NAME);
    addr->name = assignment->id->id;

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

struct Quad *statement_return_generate_tac_quads(struct Statement_return *ret) {
    struct Quad *final_expr = expr_generate_tac_quads(ret->expr);
    struct Quad *return_quad = quad_new(TAC_RETURN);
    return_quad->result = final_expr->result;
    extend_quads(return_quad);
    return return_quad;
}

struct Quad *statement_generate_tac_quads(struct Statement* stmt) {
    struct Quad *quad;
    switch(stmt->type) {
    case STMT_ASSIGN:
        quad = statement_assignment_generate_tac_quads(stmt->assignment);
        break;
    case STMT_RETURN:
        quad = statement_return_generate_tac_quads(stmt->ret);
        break;
    }
    return quad;
}

struct Quad *function_generate_tac_quads(struct Function *func) {
    // NB: we don't yet have function parameters, so this is a lot easier than it might be
    char *label = func->name->id;
    // Keep a pointer to the current last quad. Once we generate the quads for the body, we'll
    // use that pointer to find the next element, and label it accordingly.
    GList *last = g_list_last(quads);
    block_generate_tac_quads(func->body);

    GList *first_added_quad;
    if (last) {
        first_added_quad = last->next;
    } else {
        first_added_quad = quads;
    }

    struct Quad *quad;
    if (first_added_quad) {
        quad = (struct Quad *)first_added_quad->data;

    } else {
        // There's nothing here. Means we need to add a label with a NO_OP instruction.
        // The lack of return will be handled below.
        quad = quad_new(TAC_NO_OP);
    }
    quad_add_label(quad, label);

    // Check if the last quad is a return. If it isn't, add one. quads can't be empty at this point; if it
    // is for some reason, segfaulting here is fine, since there's nothing else we can do to recover.
    last = g_list_last(quads);
    struct Quad *last_quad = (struct Quad *)last->data;
    if (last_quad->instruction != TAC_RETURN) {
        struct Quad *return_quad = quad = quad_new(TAC_RETURN);
        extend_quads(return_quad);
        last_quad = return_quad;
    }

    return last_quad;
}


struct Quad *block_element_generate_tac_quads(void *elt) {
    struct Quad *quad;
    enum Node_type node_type = ast_node_type(elt);
    switch (node_type) {
    case NODE_TYPE_FUNCTION:
        quad = function_generate_tac_quads((struct Function *)elt);
        break;
    case NODE_TYPE_DECLARATION:
        quad = declaration_generate_tac_quads((struct Declaration *)elt);
        break;
    case NODE_TYPE_STATEMENT:
        quad = statement_generate_tac_quads((struct Statement*) elt);
        break;
    default:
        log_err("Received invalid node type in block_element_free: %d", node_type);
    }
    return quad;
}

void block_generate_tac_quads(struct Block *block) {
    g_list_foreach(block->block_elements, (gpointer)block_element_generate_tac_quads, NULL);
}

GList *program_generate_tac_quads(struct Program *prog) {
    block_generate_tac_quads(prog->top_level_block);
    return quads;
}
