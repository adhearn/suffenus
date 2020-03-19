#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "lang.h"
#include "util.h"

/**
 * I really don't like this, but I'm not yet familiar enough with the implications to know how
 * bad of an idea this is. The first element of every node struct is a node type, so we can treat
 * a pointer to any AST node as a pointer to the node type. This feels pretty hacky, but I need
 * some form of abstraction over node types, and nothing else fits the bill...
 */
enum NodeType ast_node_type(void *ast_node) {
    enum NodeType node_type = *(enum NodeType *)ast_node;
    return node_type;
}

// Reverse lookup for op names, useful for printing
char *OP_NAMES[] = { "+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">=" };

struct Type *type_new(char *type) {
    struct Type *t = malloc(sizeof(struct Type));
    check_mem(t);
    t->type = type;
    return t;
}

void type_free(struct Type *type) {
    free(type->type);
    free(type);
}

/* void type_print(struct Type *type) { */
/*     printf("%s", type->type); */
/* } */

/**
 * For now, we can be lazy with our function types, because functions don't yet
 * support parameters! We can just add some empty parens at the end of the type
 * string for now, but we'll have to support params later.
 */
void type_make_fn_type(struct Type *type) {
    char *old_type_str = type->type;
    asprintf(&(type->type), "%s()", old_type_str);
}

struct Identifier *identifier_new(char *id, struct Type *type) {
    struct Identifier *identifier = malloc(sizeof(struct Identifier));
    check_mem(id);
    identifier->node_type = NODE_TYPE_IDENTIFIER;
    identifier->id = id;
    identifier->type = type;
    return identifier;
}

void identifier_free(struct Identifier *id) {
    free(id->id);
    free(id);
}

/* void identifier_print(struct Identifier *id) { */
/*     printf("%s", id->id); */
/* } */

struct Constant *constant_new(int constant) {
    struct Constant *c = malloc(sizeof(struct Identifier));
    check_mem(c);
    c->node_type = NODE_TYPE_CONSTANT;
    c->val = constant;
    return c;
}

void constant_free(struct Constant *c) {
    free(c);
}

/* void constant_print(struct Constant *c) { */
/*     printf("%d", c->val); */
/* } */

struct Declaration *declaration_new(struct Type *type, struct Identifier *id) {
    struct Declaration *decl = malloc(sizeof(struct Declaration));
    check_mem(decl);
    decl->node_type = NODE_TYPE_DECLARATION;
    decl->type = type;
    decl->id = id;
    return decl;
}

void declaration_free(struct Declaration *decl) {
    type_free(decl->type);
    identifier_free(decl->id);
    free(decl);
}

/* void declaration_print(struct Declaration *decl) { */
/*     printf("Declaration: %s %s\n", decl->type->type, decl->id->id); */
/* } */

struct ExprAssignment *expr_assignment_new(struct Identifier *lhs, struct Expr *rhs) {
    struct ExprAssignment *assignment = malloc(sizeof(struct ExprAssignment));
    check_mem(assignment);
    assignment->lhs = lhs;
    assignment->rhs = rhs;
    return assignment;
}

void expr_assignment_free(struct ExprAssignment *assignment) {
    identifier_free(assignment->lhs);
    expr_free(assignment->rhs);
    free(assignment);
}

struct ExprCall *expr_call_new(struct Expr *function, GList *args) {
    struct ExprCall *call = malloc(sizeof(struct ExprCall));
    check_mem(call);
    call->function = function;
    call->args = args;
    return call;
}

void expr_call_free(struct ExprCall *call) {
    expr_free(call->function);
    g_list_foreach(call->args, (GFunc)expr_free, NULL);
    free(call);
}

struct ExprConstant *expr_constant_new(struct Constant *constant) {
    struct ExprConstant *expr_constant = malloc(sizeof(struct ExprConstant));
    check_mem(expr_constant);
    expr_constant->constant = constant;
    return expr_constant;
}

void expr_constant_free(struct ExprConstant *expr) {
    free(expr->constant);
    free(expr);
}

struct ExprIdentifier *expr_identifier_new(struct Identifier *id) {
    struct ExprIdentifier *expr_id = malloc(sizeof(struct ExprIdentifier));
    check_mem(expr_id);
    expr_id->id = id;
    return expr_id;
}

void expr_identifier_free(struct ExprIdentifier *expr) {
    free(expr->id);
    free(expr);
}

struct ExprOp *expr_op_new(enum Op op, struct Expr *arg1, struct Expr *arg2) {
    struct ExprOp *expr_op = malloc(sizeof(struct ExprOp));
    check_mem(expr_op);
    expr_op->op = op;
    expr_op->arg1 = arg1;
    expr_op->arg2 = arg2;
    return expr_op;
}

void expr_op_free(struct ExprOp *expr) {
    free(expr->arg1);
    free(expr->arg2);
    free(expr);
}

struct Expr *expr_new(enum ExprType type) {
    struct Expr *expr = malloc(sizeof(struct Expr));
    check_mem(expr);
    expr->node_type = NODE_TYPE_EXPR;
    expr->type = type;
    return expr;
}

void expr_free(struct Expr *expr) {
    switch (expr->type) {
    case (EXPR_ASSIGNMENT):
        expr_assignment_free(expr->assignment);
        break;
    case (EXPR_CALL):
        expr_free(expr->call->function);
        g_list_foreach(expr->call->args, (GFunc)expr_free, NULL);
        break;
    case (EXPR_CONSTANT):
        expr_constant_free(expr->constant);
        break;
    case (EXPR_IDENTIFIER):
        expr_identifier_free(expr->id);
        break;
    case (EXPR_BINOP):
    case (EXPR_RELOP):
        expr_op_free(expr->op);
        break;
    }

    free(expr);
}

/* void expr_print(struct Expr *expr) { */
/*     switch (expr->type) { */
/*     case (EXPR_CONSTANT): */
/*         constant_print(expr->constant->constant); */
/*         break; */
/*     case (EXPR_IDENTIFIER): */
/*         identifier_print(expr->id->id); */
/*         break; */
/*     case (EXPR_OP): */
/*         expr_print(expr->op->arg1); */
/*         printf(" %s ", OP_NAMES[expr->op->op]); */
/*         expr_print(expr->op->arg2); */
/*         break; */
/*     } */
/* } */

/* struct Statement_assignment *assignment_new(struct Identifier *id, struct Expr *expr) { */
/*     struct Statement_assignment  *assignment = malloc(sizeof(struct Statement_assignment)); */
/*     check_mem(assignment); */
/*     assignment->id = id; */
/*     assignment->expr = expr; */
/*     return assignment; */
/* } */

/* void statement_assignment_free(struct Statement_assignment *assignment) { */
/*     identifier_free(assignment->id); */
/*     expr_free(assignment->expr); */
/*     free(assignment); */
/* } */

/* struct Statement_return *return_new(struct Expr *expr) { */
/*     struct Statement_return *ret = malloc(sizeof(struct Statement_return)); */
/*     check_mem(ret); */
/*     ret->expr = expr; */
/*     return ret; */
/* } */

/* void statement_return_free(struct Statement_return *ret) { */
/*     expr_free(ret->expr); */
/*     free(ret); */
/* } */

struct StatementJump *statement_jump_new(enum StatementJumpType type, void *target) {
    struct StatementJump *jump = malloc(sizeof(struct StatementJump));
    check_mem(jump);
    jump->type = type;
    jump->expr = target;
    return jump;
}

void statement_jump_free(struct StatementJump *jump) {
    switch(jump->type) {
    case (JUMP_RETURN):
        expr_free(jump->expr);
        break;
    case (JUMP_BREAK):
    case (JUMP_CONTINUE):
        break;
    case (JUMP_GOTO):
        identifier_free(jump->identifier);
        break;
    }

    free(jump);
}

struct StatementSelection *statement_selection_new(struct Expr *test, struct Statement *conseq, struct Statement *alt) {
    struct StatementSelection *selection = malloc(sizeof(struct StatementSelection));
    check_mem(selection);
    selection->test = test;
    selection->conseq = conseq;
    selection->alt = alt;
    return selection;
}

void statement_selection_free(struct StatementSelection *selection) {
    expr_free(selection->test);
    statement_free(selection->conseq);
    if (selection->alt) {
        statement_free(selection->alt);
    }
    free(selection);
}

struct Statement *statement_new(enum StatementType type) {
    struct Statement *stmt = malloc(sizeof(struct Statement));
    check_mem(stmt);
    stmt->node_type = NODE_TYPE_STATEMENT;
    stmt->type = type;
    return stmt;
}

void statement_free(struct Statement *stmt) {
    switch(stmt->type) {
    case STMT_TYPE_COMPOUND:
        block_free(stmt->compound);
        break;
    case STMT_TYPE_RETURN:
    case STMT_TYPE_EXPR:
        expr_free(stmt->expr);
        break;
    case STMT_TYPE_JUMP:
        statement_jump_free(stmt->jump);
        break;
    case STMT_TYPE_SELECTION:
        statement_selection_free(stmt->selection);
        break;
    }
    free(stmt);
}

/* void statement_print(struct Statement *stmt) { */
/*     switch(stmt->type) { */
/*     case STMT_ASSIGN: */
/*         printf("Assignment: %s = ", stmt->assignment->id->id); */
/*         expr_print(stmt->assignment->expr); */
/*         break; */
/*     case STMT_RETURN: */
/*         printf("Return: "); */
/*         expr_print(stmt->ret->expr); */
/*         break; */
/*     } */
/*     printf("\n"); */
/* } */

struct Block *block_new(GList *block_elements, struct SymbolTable *st) {
    struct Block *block = malloc(sizeof(struct Block));
    check_mem(block);

    block->node_type = NODE_TYPE_BLOCK;
    block->block_elements = block_elements;
    block->st = st;
    return block;
}

struct Block *block_extend(struct Block *block, void *elt) {
    GList *block_elements = block->block_elements;
    block->block_elements = g_list_append(block_elements, elt);
    return block;
}

void block_element_free(void *elt) {
    // Hack: the node_type is first in all of our structs, so it doesn't matter which union field we use
    // to access the node_type - it's always in the same spot (namely, offset 0)
    enum NodeType node_type = ast_node_type(elt);
    switch (node_type) {
    case NODE_TYPE_FUNCTION:
        function_free((struct Function *)elt);
        break;
    case NODE_TYPE_DECLARATION:
        declaration_free((struct Declaration *)elt);
        break;
    case NODE_TYPE_STATEMENT:
        statement_free((struct Statement*) elt);
        break;
    default:
        log_err("Received invalid node type in block_element_free: %d", node_type);
    }
}

/* void block_element_print(void *elt) { */
/*     // Hack: the node_type is first in all of our structs, so it doesn't matter which union field we use */
/*     // to access the node_type - it's always in the same spot (namely, offset 0) */
/*     enum Node_type node_type = ast_node_type(elt); */
/*     switch (node_type) { */
/*     case NODE_TYPE_FUNCTION: */
/*         function_print((struct Function *)elt); */
/*         break; */
/*     case NODE_TYPE_DECLARATION: */
/*         declaration_print((struct Declaration *)elt); */
/*         break; */
/*     case NODE_TYPE_STATEMENT: */
/*         statement_print((struct Statement*) elt); */
/*         break; */
/*     default: */
/*         log_err("Received invalid node type in block_element_print: %d", node_type); */
/*     } */
/* } */

void block_free(struct Block *block) {
    g_list_free_full(block->block_elements, (GDestroyNotify)block_element_free);
}

/* void block_print(struct Block *block) { */
/*     g_list_foreach(block->block_elements, (GFunc)block_element_print, NULL); */
/* } */

struct Function *function_new(struct Type *return_type, struct Identifier *name, GList *param_declarations, struct Block *body) {
    struct Function *function = malloc(sizeof(struct Function));
    check_mem(function);

    function->node_type = NODE_TYPE_FUNCTION;
    function->return_type = return_type;
    function->name = name;
    function->param_declarations = param_declarations;
    function->body = body;

    return function;
}

void function_free(struct Function *func) {
    type_free(func->return_type);
    identifier_free(func->name);
    g_list_free_full(func->param_declarations, (GDestroyNotify)declaration_free);
    block_free(func->body);
    free(func);
}

/* void function_print(struct Function *func) { */
/*     printf("Function:\nName: %s\nReturn Type: %s\nParams:", func->name->id, func->return_type->type); */
/*     g_list_foreach(func->param_declarations, (GFunc)declaration_print, NULL); */
/*     printf("Body:\n"); */
/*     block_print(func->body); */
/* } */

struct Program *program_new(struct Block *block) {
    struct Program *prog = malloc(sizeof(struct Program));
    check_mem(prog);

    prog->node_type = NODE_TYPE_PROGRAM;
    prog->top_level_block = block;
    return prog;
}

void program_free(struct Program *prog) {
    block_free(prog->top_level_block);
    free(prog);
}

/* void program_print(struct Program *prog) { */
/*     printf("Program:\n"); */
/*     block_print(prog->top_level_block); */
/* } */

struct SymbolTable *symbol_table_new(struct SymbolTable* parent) {
    struct SymbolTable *st = malloc(sizeof(struct SymbolTable));
    check_mem(st);

    st->parent = parent;
    GHashTable *table = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)identifier_free);
    st->table = table;
    return st;
}

void symbol_table_free(struct SymbolTable *st) {
    g_hash_table_destroy(st->table);
    free(st);
}

void symbol_table_extend(struct SymbolTable *st, struct Identifier *id) {
    char *key = id->id;
    if (g_hash_table_contains(st->table, key)) {
        log_err("Duplicate declaration for identifier: %s\n", id->id);
    } else {
        g_hash_table_insert(st->table, id->id, id);
    }
}

struct Identifier *symbol_table_lookup(struct SymbolTable *st, char *key) {
    struct SymbolTable *cur = st;
    do {
        struct Identifier *match = (struct Identifier *)g_hash_table_lookup(cur->table, key);
        if (match) {
            return match;
        }
    } while ((cur = cur->parent));
    return NULL;
}

/**
 * Lookup a name in the symbol table without recursively traversing parent tables.
 */
struct Identifier *symbol_table_lookup_local(struct SymbolTable *st, char *key) {
    struct Identifier *match = (struct Identifier *)g_hash_table_lookup(st->table, key);
    return match;
}
