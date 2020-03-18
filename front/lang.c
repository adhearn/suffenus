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
enum Node_type ast_node_type(void *ast_node) {
    enum Node_type node_type = *(enum Node_type *)ast_node;
    return node_type;
}

// Reverse lookup for op names, useful for printing
char *OP_NAMES[] = { "+", "-", "*", "/", "=", "!=", "<", ">", "<=", ">=" };

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

void type_print(struct Type *type) {
    printf("%s", type->type);
}

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

void identifier_print(struct Identifier *id) {
    printf("%s", id->id);
}

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

void constant_print(struct Constant *c) {
    printf("%d", c->val);
}

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

void declaration_print(struct Declaration *decl) {
    printf("Declaration: %s %s\n", decl->type->type, decl->id->id);
}


struct Expr_op *expr_op_new(enum Op op, struct Expr *arg1, struct Expr *arg2) {
    struct Expr_op *expr_op = malloc(sizeof(struct Expr_op));
    check_mem(expr_op);
    expr_op->op = op;
    expr_op->arg1 = arg1;
    expr_op->arg2 = arg2;
    return expr_op;
}

void expr_op_free(struct Expr_op *expr) {
    free(expr->arg1);
    free(expr->arg2);
    free(expr);
}

struct Expr_identifier *expr_identifier_new(struct Identifier *id) {
    struct Expr_identifier *expr_id = malloc(sizeof(struct Expr_identifier));
    check_mem(expr_id);
    expr_id->id = id;
    return expr_id;
}

void expr_identifier_free(struct Expr_identifier *expr) {
    free(expr->id);
    free(expr);
}

struct Expr_constant *expr_constant_new(struct Constant *constant) {
    struct Expr_constant *expr_constant = malloc(sizeof(struct Expr_constant));
    check_mem(expr_constant);
    expr_constant->constant = constant;
    return expr_constant;
}

void expr_constant_free(struct Expr_constant *expr) {
    free(expr->constant);
    free(expr);
}

struct Expr *expr_new(enum Expr_type type) {
    struct Expr *expr = malloc(sizeof(struct Expr));
    check_mem(expr);
    expr->node_type = NODE_TYPE_EXPR;
    expr->type = type;
    return expr;
}

void expr_free(struct Expr *expr) {
    switch (expr->type) {
    case (EXPR_CONSTANT):
        expr_constant_free(expr->constant);
        break;
    case (EXPR_IDENTIFIER):
        expr_identifier_free(expr->id);
        break;
    case (EXPR_OP):
        expr_op_free(expr->op);
        break;
    }

    free(expr);
}

void expr_print(struct Expr *expr) {
    switch (expr->type) {
    case (EXPR_CONSTANT):
        constant_print(expr->constant->constant);
        break;
    case (EXPR_IDENTIFIER):
        identifier_print(expr->id->id);
        break;
    case (EXPR_OP):
        expr_print(expr->op->arg1);
        printf(" %s ", OP_NAMES[expr->op->op]);
        expr_print(expr->op->arg2);
        break;
    }
}

struct Statement_assignment *assignment_new(struct Identifier *id, struct Expr *expr) {
    struct Statement_assignment  *assignment = malloc(sizeof(struct Statement_assignment));
    check_mem(assignment);
    assignment->id = id;
    assignment->expr = expr;
    return assignment;
}

void statement_assignment_free(struct Statement_assignment *assignment) {
    identifier_free(assignment->id);
    expr_free(assignment->expr);
    free(assignment);
}

struct Statement_return *return_new(struct Expr *expr) {
    struct Statement_return *ret = malloc(sizeof(struct Statement_return));
    check_mem(ret);
    ret->expr = expr;
    return ret;
}

void statement_return_free(struct Statement_return *ret) {
    expr_free(ret->expr);
    free(ret);
}

struct Statement *statement_new(enum Statement_type type) {
    struct Statement *stmt = malloc(sizeof(struct Statement));
    check_mem(stmt);
    stmt->node_type = NODE_TYPE_STATEMENT;
    stmt->type = type;
    return stmt;
}

void statement_free(struct Statement *stmt) {
    switch(stmt->type) {
    case STMT_ASSIGN:
        statement_assignment_free(stmt->assignment);
        break;
    case STMT_RETURN:
        statement_return_free(stmt->ret);
        break;
    }
    free(stmt);
}

void statement_print(struct Statement *stmt) {
    switch(stmt->type) {
    case STMT_ASSIGN:
        printf("Assignment: %s = ", stmt->assignment->id->id);
        expr_print(stmt->assignment->expr);
        break;
    case STMT_RETURN:
        printf("Return: ");
        expr_print(stmt->ret->expr);
        break;
    }
    printf("\n");
}

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
    enum Node_type node_type = ast_node_type(elt);
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

void block_element_print(void *elt) {
    // Hack: the node_type is first in all of our structs, so it doesn't matter which union field we use
    // to access the node_type - it's always in the same spot (namely, offset 0)
    enum Node_type node_type = ast_node_type(elt);
    switch (node_type) {
    case NODE_TYPE_FUNCTION:
        function_print((struct Function *)elt);
        break;
    case NODE_TYPE_DECLARATION:
        declaration_print((struct Declaration *)elt);
        break;
    case NODE_TYPE_STATEMENT:
        statement_print((struct Statement*) elt);
        break;
    default:
        log_err("Received invalid node type in block_element_print: %d", node_type);
    }
}

void block_free(struct Block *block) {
    g_list_free_full(block->block_elements, (GDestroyNotify)block_element_free);
}

void block_print(struct Block *block) {
    g_list_foreach(block->block_elements, (GFunc)block_element_print, NULL);
}

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

void function_print(struct Function *func) {
    printf("Function:\nName: %s\nReturn Type: %s\nParams:", func->name->id, func->return_type->type);
    g_list_foreach(func->param_declarations, (GFunc)declaration_print, NULL);
    printf("Body:\n");
    block_print(func->body);
}

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

void program_print(struct Program *prog) {
    printf("Program:\n");
    block_print(prog->top_level_block);
}

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
