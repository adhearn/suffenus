#include <stdio.h>
#include <glib.h>
#include "util.h"
#include "lang.h"
#include "icg.h"

GList *quads;

void extend_quads(struct Quad *quad) {
    quads = g_list_append(quads, quad);
}

char *tmp_var_new(char *base) {
    if (!base) {
        base = "TMP";
    }

    static int count = 0;
    char **tmp = malloc(sizeof(char *));
    check_mem(tmp);
    asprintf(tmp, "%s%d", base, count);
    count++;
    return *tmp;
}

struct Address *label_new_named(char *name) {
    struct Address *addr = address_new(ADDR_TYPE_LABEL);
    addr->name = name;
    return addr;
}

struct Address *label_new() {
    char *tmp_label = tmp_var_new("L");
    return label_new_named(tmp_label);
}

struct Address *label_from_identifier(struct Identifier *identifier) {
    return label_new_named(identifier->id);
}

struct Address *address_new(enum TACAddressType type) {
    struct Address *addr = malloc(sizeof(struct Address));
    check_mem(addr);
    addr->type = type;
    return addr;
}

struct Address *address_temp_new() {
    struct Address *addr = address_new(ADDR_TYPE_TEMP);
    addr->name = tmp_var_new("TMP");
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
    case (ADDR_TYPE_FUNC_PARAM):
        printf("PARAM %d", addr->value);
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

struct Quad *quad_labeled_noop_new(struct Address *label) {
    struct Quad *quad = quad_new(TAC_NO_OP);
    quad_add_label(quad, label);
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
    case (TAC_CALL):
        address_print(quad->result);
        printf(" = CALL ");
        address_print(quad->arg1);
        printf(" ");
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
    case (TAC_PARAM):
        printf("PARAM ");
        address_print(quad->result);
        break;
    case (TAC_RETURN):
        printf("RETURN ");
        address_print(quad->result);
        break;
    case (TAC_IF):
        printf("IF ");
        address_print(quad->arg1);
        printf(" %s ", OP_NAMES[quad->op]);
        address_print(quad->arg2);
        printf(" JUMP ");
        address_print(quad->result);
        break;
    case (TAC_CASE):
        printf("CASE ");
        address_print(quad->arg1);
        printf(" ");
        address_print(quad->arg2);
        printf(" ");
        address_print(quad->result);
        break;
    case (TAC_COPY_INDEXED_LHS):
        printf("LINDEXED ");
        address_print(quad->arg1);
        printf(" ");
        address_print(quad->arg2);
        printf(" ");
        address_print(quad->result);
        break;
    case (TAC_COPY_INDEXED_RHS):
        printf("RINDEXED ");
        address_print(quad->result);
        printf(" ");
        address_print(quad->arg1);
        printf(" ");
        address_print(quad->arg2);
        break;
    }
    printf("\n");
}

struct Quad *expr_assignment_identifier_generate_tac_quads_rvalue(struct Identifier *lhs, struct Expr *rhs) {
    struct Quad *final_expr = expr_generate_tac_quads_rvalue(rhs);
    struct Address *addr = address_new(ADDR_TYPE_NAME);
    addr->name = lhs->id;

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

struct Address *index_base(struct ExprIndexed *indexed) {
    struct Address *addr;
    switch (indexed->expr->type) {
    case (EXPR_IDENTIFIER):
        addr = address_new(ADDR_TYPE_NAME);
        addr->name = indexed->expr->id->id->id;
        break;
    default:
        log_err("Invalid base expression type for indexed expression: %d", indexed->expr->type);
        return NULL;
    }
    return addr;
}

/**
 * Generate and emit the quads for the index expr, then calculate an offset. For now, I'm keeping this incredibly
 * simple by only supporting 1d arrays. It feels like once I add more typing information, it shouldn't be too bad
 * to support multi-dimensional arrays either, but it's a bit dicey now.
 */
struct Quad *expr_indexed_calculate_offset(struct ExprIndexed *indexed) {
    return expr_generate_tac_quads_rvalue(indexed->index);
}

struct Quad *expr_assignment_indexed_generate_tac_quads_rvalue(struct ExprIndexed *lhs, struct Expr *rhs) {
    struct Quad *rhs_result = expr_generate_tac_quads_rvalue(rhs);
    struct Quad *offset = expr_indexed_calculate_offset((struct ExprIndexed *)lhs);
    struct Quad *copy = quad_new(TAC_COPY_INDEXED_LHS);
    copy->result = rhs_result->result;
    copy->arg1 = index_base(lhs);
    copy->arg2 = offset->result;
    extend_quads(copy);
    return copy;
}

struct Quad *expr_assignment_generate_tac_quads_rvalue(struct ExprAssignment *assignment) {
    struct Quad *quad;
    switch (assignment->lvalue_type) {
    case LVALUE_IDENTIFIER:
        quad = expr_assignment_identifier_generate_tac_quads_rvalue((struct Identifier *)assignment->lhs, assignment->rhs);
        break;
    case LVALUE_INDEXED:
        quad = expr_assignment_indexed_generate_tac_quads_rvalue((struct ExprIndexed *)assignment->lhs, assignment->rhs);
        break;
    }
    return quad;
}

// We don't extend quads with the param quads we create in order to make a slightly more aesthetically pleasing
// TAC output. It's unnecessary, but should make debugging the interpreter a bit easier, if nothing else.
// However, it does mean that the call function has to be a bit more complicated.
struct Quad *expr_generate_tac_quads_params(struct Expr *expr) {
    struct Quad *expr_final = expr_generate_tac_quads_rvalue(expr);
    struct Quad *param = quad_new(TAC_PARAM);
    param->result = expr_final->result;
    return param;
}

struct Quad *expr_call_generate_tac_quads_rvalue(struct ExprCall *call) {
    // The ordering is a bit wonky here. We want to generate all the quads required to evaluate the params, then
    // all the quads to identify the function. Then we actually add the param instructions, followed by the
    // actual call instruction. Not strictly required, but should make debugging TAC by hand a bit easier.
    struct Quad *function = expr_generate_tac_quads_rvalue(call->function);
    int num_args = g_list_length(call->args);
    struct Quad *params[num_args];
    GList *args = call->args;
    int i = 0;
    while (args) {
        struct Expr *arg = args->data;
        struct Quad *param = expr_generate_tac_quads_params(arg);
        params[i] = param;
        args = args->next;
        i++;
    }

    for (int i = 0; i < num_args; i++) {
        extend_quads(params[i]);
    }

    struct Quad *quad_call = quad_new(TAC_CALL);
    quad_call->arg1 = function->result;
    struct Address *addr_num_params = address_zero_new();
    addr_num_params->value = num_args;
    quad_call->arg2 = addr_num_params;
    quad_call->result = address_temp_new();
    extend_quads(quad_call);
    return quad_call;
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

struct Quad *expr_indexed_generate_tac_quads_rvalue(struct ExprIndexed *indexed) {
    struct Quad *expr_quad = expr_generate_tac_quads_rvalue(indexed->expr);
    struct Quad *index_quad = expr_generate_tac_quads_rvalue(indexed->index);
    struct Address *tmp = address_temp_new();
    struct Quad *quad = quad_new(TAC_COPY_INDEXED_RHS);
    quad->arg1 = expr_quad->result;
    quad->arg2 = index_quad->result;
    quad->result = tmp;
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
    case (EXPR_CALL):
        quad = expr_call_generate_tac_quads_rvalue(expr->call);
        break;
    case (EXPR_CONSTANT):
        quad = expr_constant_generate_tac_quads_rvalue(expr->constant);
        break;
    case (EXPR_IDENTIFIER):
        quad = expr_identifier_generate_tac_quads_rvalue(expr->id);
        break;
    case (EXPR_INDEXED):
        quad = expr_indexed_generate_tac_quads_rvalue(expr->indexed);
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
    struct Quad *quad_true = quad_new(TAC_IF);
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
    struct Quad *quad = expr_assignment_generate_tac_quads_rvalue(assignment);
    return address_generate_conditional_jump(quad->result, true_label, false_label);
}

struct Quad *expr_identifier_generate_tac_quads_jump(struct ExprIdentifier *identifier, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Address *identifier_address = address_new(ADDR_TYPE_NAME);
    identifier_address->name = identifier->id->id;
    return address_generate_conditional_jump(identifier_address, true_label, false_label);
}

struct Quad *expr_indexed_generate_tac_quads_jump(struct ExprIndexed *indexed, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Quad *quad = expr_indexed_generate_tac_quads_rvalue(indexed);
    return address_generate_conditional_jump(quad->result, true_label, false_label);
}

struct Quad *expr_call_generate_tac_quads_jump(struct ExprCall *call, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Quad *quad = expr_call_generate_tac_quads_rvalue(call);
    return address_generate_conditional_jump(quad->result, true_label, false_label);
}

struct Quad *expr_binop_generate_tac_quads_jump(struct ExprOp *op, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Quad *quad = expr_op_generate_tac_quads_rvalue(op);
    return address_generate_conditional_jump(quad->result, true_label, false_label);
}

struct Quad *expr_relop_generate_tac_quads_jump(struct ExprOp *op, struct Address *true_label, struct Address *false_label, struct Address *next_label) {
    struct Quad *arg1_quad = expr_generate_tac_quads_rvalue(op->arg1);
    struct Quad *arg2_quad = expr_generate_tac_quads_rvalue(op->arg2);
    struct Quad *relop_quad = quad_new(TAC_IF);
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
    case (EXPR_CALL):
        quad = expr_call_generate_tac_quads_jump(expr->call, true_label, false_label, next_label);
        break;
    case (EXPR_CONSTANT):
        quad = expr_constant_generate_tac_quads_jump(expr->constant, true_label, false_label, next_label);
        break;
    case (EXPR_IDENTIFIER):
        quad = expr_identifier_generate_tac_quads_jump(expr->id, true_label, false_label, next_label);
        break;
    case (EXPR_INDEXED):
        quad = expr_indexed_generate_tac_quads_jump(expr->indexed, true_label, false_label, next_label);
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

struct Quad *declarator_generate_tac_quads(struct Declarator *decl) {
    struct Quad *quad;
    if (decl->initializer) {
        switch (decl->type) {
        case DECLARATOR_IDENTIFIER: {
            struct Quad *init = expr_generate_tac_quads_rvalue(decl->initializer);
            struct Quad *copy = quad_new(TAC_COPY);
            struct Address *addr = address_new(ADDR_TYPE_NAME);
            addr->name = declarator_identifier(decl)->id;
            copy->result = addr;
            copy->arg1 = init->result;
            extend_quads(copy);
            quad = copy;
        }
        case DECLARATOR_FUNCTION:
        case DECLARATOR_ARRAY:
        default:
            break;
        }
    }
    return quad;
}

struct Quad *declaration_generate_tac_quads(struct Declaration *decl) {
    // We only care about initializers here
    struct Quad *quad;
    GList *declarators = decl->declarators;
    while (declarators) {
        struct Quad *res = declarator_generate_tac_quads(declarators->data);
        if (res) {
            quad = res;
        }
        declarators = declarators->next;
    }

    if (quad) {
        return quad;
    } else {
        struct Quad *noop = quad_new(TAC_NO_OP);
        extend_quads(noop);
        return noop;
    }
}

struct Quad *statement_for_generate_tac_quads(struct StatementFor *loop, struct Address *rejoin_label, struct Address *outer_loop_start, struct Address *outer_loop_end) {
    struct Address *start_label = label_new_named("LOOP_START");
    struct Address *test_label = label_new_named("LOOP_TEST");
    struct Address *end_label = label_new_named("LOOP_END");
    struct Address *body_label = label_new_named("LOOP_BODY");
    struct Quad *noop;

    expr_generate_tac_quads_rvalue(loop->init);

    if (loop->update) {
        // Need to skip over the update for the initial iteration through the loop
        struct Quad *skip_update = quad_new(TAC_JUMP);
        skip_update->result = test_label;
        extend_quads(skip_update);
    }

    noop = quad_new(TAC_NO_OP);
    quad_add_label(noop, start_label);
    extend_quads(noop);

    if (loop->update) {
        expr_generate_tac_quads_rvalue(loop->update);
        //noop = quad_labeled_noop_new(test_label);
        noop = quad_new(TAC_NO_OP);
        quad_add_label(noop, test_label);
        extend_quads(noop);
    }

    expr_generate_tac_quads_jump(loop->test, body_label, end_label, NULL);

    // noop = quad_labeled_noop_new(body_label);
    noop = quad_new(TAC_NO_OP);
    quad_add_label(noop, body_label);
    extend_quads(noop);
    statement_generate_tac_quads(loop->body, rejoin_label, start_label, end_label);

    struct Quad *jump_to_start = quad_new(TAC_JUMP);
    jump_to_start->result = start_label;
    extend_quads(jump_to_start);

    // noop = quad_labeled_noop_new(end_label);
    noop = quad_new(TAC_NO_OP);
    quad_add_label(noop, end_label);
    extend_quads(noop);
    return noop;
}

struct Quad *statement_jump_generate_tac_quads(struct StatementJump *jump, struct Address *loop_start, struct Address *loop_end) {
    struct Quad *quad;
    switch (jump->type) {
    case JUMP_BREAK: {
        quad = quad_new(TAC_JUMP);
        if (!loop_end) {
            log_err("Received null loop_end for a break statement, probably an error");
        }
        quad->result = loop_end;
        extend_quads(quad);
        break;
    }
    case JUMP_CONTINUE: {
        quad = quad_new(TAC_JUMP);
        if (!loop_start) {
            log_err("Received null loop_start for a continue statement, probably an error");
        }
        quad->result = loop_start;
        extend_quads(quad);
        break;
    }
    case JUMP_GOTO:
        quad = quad_new(TAC_JUMP);
        quad->result = label_from_identifier(jump->identifier);
        extend_quads(quad);
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

struct Quad *statement_labeled_generate_tac_quads(struct StatementLabeled *labeled, struct Address *rejoin_label, struct Address *loop_start, struct Address *loop_end) {
    struct Quad *quad;

    switch (labeled->type) {
        // These should only ever be processed in the context of a switch, so ignore them here
    case LABELED_CASE:
    case LABELED_DEFAULT:
        break;
    case LABELED_LABEL: {
        struct Address *label_addr = address_new(ADDR_TYPE_LABEL);
        label_addr->name = labeled->label->id;
        struct Quad *noop = quad_labeled_noop_new(label_addr);
        extend_quads(noop);
        quad = statement_generate_tac_quads(labeled->statement, rejoin_label, loop_start, loop_end);
        break;
    }
    }

    return quad;
}

    struct Quad *statement_selection_generate_tac_quads(struct StatementSelection *selection, struct Address *rejoin_label, struct Address *loop_start, struct Address *loop_end) {
    /**
     * One of the more complicated cases. The gist is that we have to generate 2 possible labels:
     * one for the true case, one for the false case. The third label will  point to where the branches
     * rejoin, and is passed in as an arg. (Because this case is recursive, we can't generate it here,
     * it has to be passed in.) NOTE: as currently designed, this generates a ton of extra No-Ops. This
     * is a prime spot for optimization.
     */
    struct Quad *quad;
    struct Address *true_label = label_new();
    struct Quad *true_noop = quad_labeled_noop_new(true_label);

    struct Address *false_label;

    if (selection->alt) {
        false_label = label_new();
    } else {
        false_label = rejoin_label;
    }

    expr_generate_tac_quads_jump(selection->test, true_label, false_label, rejoin_label);
    extend_quads(true_noop);
    quad = statement_generate_tac_quads(selection->conseq, rejoin_label, loop_start, loop_end);

    if (selection->alt) {
        struct Quad *false_noop = quad_labeled_noop_new(false_label);
        extend_quads(false_noop);
        quad = statement_generate_tac_quads(selection->alt, rejoin_label, loop_start, loop_end);
    }
    return quad;
}

GList *extract_labeled_statements(struct Statement *switch_body) {
    GList *labeled_statements = NULL;
    switch (switch_body->type) {
    case STMT_TYPE_COMPOUND: {
        // For each statement in the compound statement's body, add it to the list if it is a labeled statement
        GList *block_elements = switch_body->compound->block_elements;
        while (block_elements) {
            if (ast_node_type(block_elements->data) == NODE_TYPE_STATEMENT) {
                struct Statement *stmt = (struct Statement *)block_elements->data;
                if (stmt->type == STMT_TYPE_LABELED) {
                    labeled_statements = g_list_append(labeled_statements, stmt->labeled);
                }
            }
            block_elements = block_elements->next;
        }
        break;
    }
    case STMT_TYPE_LABELED:
        labeled_statements = g_list_append(labeled_statements, switch_body->labeled);
        break;
    default:
        // avoid a compiler warning...
        break;
    }

    return labeled_statements;
}

/**
 * For future reference, this is the rough algorithm we're following:
 * 1. Eval the test case
 * 2. Create labels for END and TESTS
 * 3. JUMP to TESTS
 * 4. For each labeled statement:
 *    a. Create a label for this case
 *    b. Add label and test expr to a queue to handle later
 *    c. Emit the case label
 *    d. Recursively process the statement body, passing in the end label as the "loop_end" (TODO: rename this)
 * 5. Emit TESTS label
 * 6. For each item in the queue:
 *    a. Generate quads for the case expression (unless it's null, i.e. it's for the default case)
 *    b. Generate a TAC_CASE quad, unless the expr is null, in which case just JUMP to the given label
 * 7. Emit the END label
 */
struct Quad *statement_switch_generate_tac_quads(struct StatementSwitch* switch_statement, struct Address *rejoin_label, struct Address *loop_start, struct Address *loop_end) {
    // The semantics of this one are a bit weird. The body of a switch statement is a statement, not a list
    // of case statements. However, only case statements will be run, unless there's a goto that jumps to
    // a label inside the switch (although why you would do that is beyond me). At this point, we've already set
    // up the symbol table in the analysis pass, so we can ignore anything other than labeled statements.
    struct Statement *body = switch_statement->body;
    GList *case_queue = NULL;
    GList *labeled_statements = extract_labeled_statements(body);
    if (!labeled_statements) {
        log_err("Switch statement with no case statements");
        return quad_new(TAC_NO_OP);
    }

    // 1. Eval the test case
    struct Quad *test = expr_generate_tac_quads_rvalue(switch_statement->test);

    // 2. Create labels for END and TESTS
    struct Address *tests_label = label_new();
    struct Address *end_label = label_new();

    // 3. Jump to TESTS
    struct Quad *jump_to_tests = quad_new(TAC_JUMP);
    jump_to_tests->result = tests_label;
    extend_quads(jump_to_tests);

    // 4. For each labeled statement:
    while (labeled_statements) {
        struct StatementLabeled *labeled_statement = labeled_statements->data;
        if (labeled_statement->type == LABELED_CASE || labeled_statement->type == LABELED_DEFAULT) {
            // 4a. Create a label for this case
            struct Address *case_label = label_new();

            // 4b. Add label and expr to queue to handle later
            struct SwitchQueuePair *pair = malloc(sizeof(struct SwitchQueuePair));
            if (labeled_statement->type == LABELED_CASE) {
                pair->test = labeled_statement->test;
            } else {
                // For the default, we set the test to null. When we create this quad later, we will
                // know to just do a jump, rather than a test.
                pair->test = NULL;
            }
            pair->label = case_label;
            case_queue = g_list_append(case_queue, pair);

            // 4c. Emit the case label
            struct Quad *case_noop = quad_labeled_noop_new(case_label);
            extend_quads(case_noop);

            // 4d. Recursively process the statement
            statement_generate_tac_quads(labeled_statement->statement, rejoin_label, loop_start, end_label);
        } else {
            // Labels can just show up inside switch statements. I think it's dumb to allow, but that's
            // how the language works. There's probably some brilliant use case for it...
            statement_labeled_generate_tac_quads(labeled_statement, rejoin_label, loop_start, end_label);
        }
        labeled_statements = labeled_statements->next;
    }

    // Regardless of any breaks that occurred in the actual bodies of the case statements,
    // we need to jump to the end, otherwise we'll end up looping if there isn't a break
    struct Quad *jump_to_end = quad_new(TAC_JUMP);
    jump_to_end->result = end_label;
    extend_quads(jump_to_end);

    // Emit TESTS label
    extend_quads(quad_labeled_noop_new(tests_label));

    // For each item in the queue:
    while (case_queue) {
        struct SwitchQueuePair *pair = case_queue->data;

        if (pair->test) {
            // Generate quads for the case expression
            struct Quad *case_test = expr_generate_tac_quads_rvalue(pair->test);

            // Generate TAC_CASE quad ...
            struct Quad *case_quad = quad_new(TAC_CASE);
            case_quad->result = pair->label;
            case_quad->arg1 = test->result;
            case_quad->arg2 = case_test->result;
            extend_quads(case_quad);
        } else {
            // ... unless it's the default case
            struct Quad *jump = quad_new(TAC_JUMP);
            jump->result = pair->label;
            extend_quads(jump);
        }

        case_queue = case_queue->next;
    }
    struct Quad *end_label_noop = quad_labeled_noop_new(end_label);
    extend_quads(end_label_noop);
    return end_label_noop;
}

struct Quad *statement_while_generate_tac_quads(struct StatementWhile *loop, struct Address *rejoin_label, struct Address *outer_loop_start, struct Address *outer_loop_end) {
    struct Address *start_label = label_new();
    struct Address *end_label = label_new();
    struct Address *body_label = label_new();
    struct Quad *noop = quad_labeled_noop_new(start_label);
    extend_quads(noop);
    expr_generate_tac_quads_jump(loop->test, body_label, end_label, NULL);

    noop = quad_labeled_noop_new(body_label);
    extend_quads(noop);
    statement_generate_tac_quads(loop->body, rejoin_label, start_label, end_label);

    struct Quad *jump_to_start = quad_new(TAC_JUMP);
    jump_to_start->result = start_label;
    extend_quads(jump_to_start);

    noop = quad_labeled_noop_new(end_label);
    extend_quads(noop);
    return noop;
}

struct Quad *statement_generate_tac_quads(struct Statement *stmt, struct Address *rejoin_label, struct Address *loop_start, struct Address *loop_end) {
    struct Quad *quad;
    switch(stmt->type) {
    case (STMT_TYPE_COMPOUND):
        quad = block_generate_tac_quads(stmt->compound, loop_start, loop_end);
        break;
    case (STMT_TYPE_EXPR):
        quad = expr_generate_tac_quads_rvalue(stmt->expr);
        break;
    case (STMT_TYPE_FOR):
        quad = statement_for_generate_tac_quads(stmt->for_loop, rejoin_label, loop_start, loop_end);
        break;
    case (STMT_TYPE_JUMP):
        quad = statement_jump_generate_tac_quads(stmt->jump, loop_start, loop_end);
        break;
    case (STMT_TYPE_LABELED): {
        struct StatementLabeled *labeled = stmt->labeled;
        quad = statement_labeled_generate_tac_quads(labeled, rejoin_label, loop_start, loop_end);
        break;
    }
    case (STMT_TYPE_RETURN):
        quad = expr_generate_tac_quads_rvalue(stmt->expr);
        break;
    case (STMT_TYPE_SELECTION):
        quad = statement_selection_generate_tac_quads(stmt->selection, rejoin_label, loop_start, loop_end);
        break;
    case (STMT_TYPE_SWITCH):
        quad = statement_switch_generate_tac_quads(stmt->switch_statement, rejoin_label, loop_start, loop_end);
        break;
    case (STMT_TYPE_WHILE):
        quad = statement_while_generate_tac_quads(stmt->while_loop, rejoin_label, loop_start, loop_end);
        break;
    }
    return quad;
}

struct Quad *function_generate_tac_quads(struct Function *func) {
    struct Quad *function_start_noop = quad_labeled_noop_new(label_new_named(function_identifier(func)->id));
    extend_quads(function_start_noop);

    GList *args = func->declarator->param_or_identifier_list;
    int i = 0;
    while (args) {
        // Create address for the param name
        struct Declaration *param_declaration = (struct Declaration *)args->data;
        // param declarations can only have a single declarator, so we're safe to just access the first one
        struct Declarator *declarator = g_list_first(param_declaration->declarators)->data;
        struct Identifier *identifier = declarator_identifier(declarator);
        struct Address *arg_addr = address_new(ADDR_TYPE_NAME);
        arg_addr->name = identifier->id;

        // Create Address for the param itself
        struct Address *param_addr = address_new(ADDR_TYPE_FUNC_PARAM);
        param_addr->value = i;

        // Create quad to assign the param to the name
        struct Quad *quad = quad_new(TAC_COPY);
        quad->result = arg_addr;
        quad->arg1 = param_addr;

        // Add it to the quads list
        extend_quads(quad);

        // Update loop variables
        i++;
        args = args->next;
    }

    block_generate_tac_quads(func->body, NULL, NULL);

    GList *last = g_list_last(quads);
    if (last) {
        return last->data;
    } else {
        return NULL;
    }
}


struct Quad *block_element_generate_tac_quads(void *elt, struct Address *loop_start, struct Address *loop_end) {
    struct Quad *quad;
    enum NodeType node_type = ast_node_type(elt);
    switch (node_type) {
    case NODE_TYPE_FUNCTION:
        quad = function_generate_tac_quads((struct Function *)elt);
        break;
    case NODE_TYPE_DECLARATION:
        quad = declaration_generate_tac_quads((struct Declaration *)elt);
        break;
    case NODE_TYPE_STATEMENT: {
        struct Address *rejoin_label = label_new();
        struct Quad *rejoin_quad = quad_new(TAC_NO_OP);
        quad_add_label(rejoin_quad, rejoin_label);
        quad = statement_generate_tac_quads((struct Statement*) elt, rejoin_label, loop_start, loop_end);
        extend_quads(rejoin_quad);
        quad = rejoin_quad;
        break;
    }
    default:
        log_err("Received invalid node type in block_element_free: %d", node_type);
    }
    return quad;
}

struct Quad *block_generate_tac_quads(struct Block *block, struct Address *loop_start, struct Address *loop_end) {
    struct Quad *quad;
    GList *elt = block->block_elements;
    while (elt) {
        quad = block_element_generate_tac_quads(elt->data, loop_start, loop_end);
        elt = elt->next;
    }
    return quad;
}

GList *program_generate_tac_quads(struct Program *prog) {
    block_generate_tac_quads(prog->top_level_block, NULL, NULL);
    return quads;
}
