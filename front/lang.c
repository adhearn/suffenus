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
char *OP_NAMES[] = { "+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">=", "*", "&"};

struct Type *type_new(enum TypeSpecifier type) {
    struct Type *t = malloc(sizeof(struct Type));
    check_mem(t);
    t->type = type;
    return t;
}

void type_free(struct Type *type) {
    free(type);
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

struct Declarator *declarator_new(enum DeclaratorType type, struct Declarator *sub_declarator, void *other) {
    struct Declarator *declarator = malloc(sizeof(struct Declarator));
    check_mem(declarator);
    declarator->type = type;
    declarator->sub_declarator = sub_declarator;
    declarator->identifier = other;
    return declarator;
}

struct Identifier *declarator_identifier(struct Declarator *declarator) {
    switch (declarator->type) {
    case (DECLARATOR_IDENTIFIER):
        return declarator->identifier;
    case DECLARATOR_FUNCTION:
    case DECLARATOR_ARRAY:
        return declarator_identifier(declarator->sub_declarator);
    default:
        return NULL;
    }
}

void declarator_free(struct Declarator *declarator) {
    return;
}

struct Declaration *declaration_new(GList *specifiers, GList *declarators) {
    struct Declaration *decl = malloc(sizeof(struct Declaration));
    check_mem(decl);
    decl->node_type = NODE_TYPE_DECLARATION;
    decl->specifiers = specifiers;
    decl->declarators = declarators;
    return decl;
}

void declaration_free(struct Declaration *decl) {
// TODO: implement this
    free(decl);
}

struct Enum *enum_new(struct Identifier *identifier, GList *enumerators) {
    struct Enum *e = malloc(sizeof(struct Enum));
    check_mem(e);
    e->identifier = identifier;
    e->enumerators = enumerators;
    return e;
}

struct EnumElement *enum_element_new(struct Identifier *identifier, struct Expr *expr) {
    struct EnumElement *elt = malloc(sizeof(struct EnumElement));
    check_mem(elt);
    elt->identifier = identifier;
    elt->expr = expr;
    return elt;
}

struct Struct *struct_new(struct Identifier *identifier, GList *declarations) {
    struct Struct *s = malloc(sizeof(struct Struct));
    check_mem(s);
    s->identifier = identifier;
    s->declarations = declarations;
    return s;
}

struct ExprAssignment *expr_assignment_new(enum LValueType type, void *lhs, struct Expr *rhs) {
    struct ExprAssignment *assignment = malloc(sizeof(struct ExprAssignment));
    check_mem(assignment);
    assignment->lvalue_type = type;
    assignment->lhs = lhs;
    assignment->rhs = rhs;
    return assignment;
}

void expr_assignment_free(struct ExprAssignment *assignment) {
    switch (assignment->lvalue_type) {
    case LVALUE_IDENTIFIER:
        identifier_free((struct Identifier *)assignment->lhs);
        break;
    case LVALUE_INDEXED:
        expr_indexed_free((struct ExprIndexed *)assignment->lhs);
        break;
    case LVALUE_POINTER:
        expr_indexed_free((struct ExprIndexed *)assignment->lhs);
        // TODO: Free the GList too (I'm in a hurry, so not doing it now)
        break;
    }
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

struct ExprIndexed *expr_indexed_new(struct Expr *expr, struct Expr *index) {
    struct ExprIndexed *expr_indexed = malloc(sizeof(struct ExprIndexed));
    check_mem(expr_indexed);
    expr_indexed->expr = expr;
    expr_indexed->index = index;
    return expr_indexed;
}

void expr_indexed_free(struct ExprIndexed *expr) {
    free(expr->expr);
    free(expr->index);
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
    if (expr->arg2) {
        free(expr->arg2);
    }
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
    case (EXPR_INDEXED):
        expr_indexed_free(expr->indexed);
        break;
    case (EXPR_BINOP):
    case (EXPR_RELOP):
    case (EXPR_UNOP):
        expr_op_free(expr->op);
        break;
    }

    free(expr);
}

// NOTE: This will return bad answers if given a non-constant expr.
// The obvious solution is to use a loop-based DFS, but that's so inelegant - eval *wants* to be recursive.
// I could also use pointers to pass in a solution, then return an error code, or use a global, or ...
// plenty of options, but this is good enough for now.
int expr_eval_constant(struct Expr *expr) {
    switch(expr->type) {
    case EXPR_BINOP: {
        int lhs = expr_eval_constant(expr->op->arg1);
        int rhs = expr_eval_constant(expr->op->arg2);
        switch (expr->op->op) {
        case Op_add:
            return lhs + rhs;
        case Op_sub:
            return lhs - rhs;
        case Op_mul:
            return lhs * rhs;
        case Op_div:
            return lhs / rhs;
        default:
            log_err("Attempted to constant eval an expression wiht a bad op: %d", expr->op->op);
        }
        break;
    }
    case EXPR_CONSTANT:
        return expr->constant->constant->val;
        break;
    case EXPR_ASSIGNMENT:
    case EXPR_CALL:
    case EXPR_IDENTIFIER:
    case EXPR_INDEXED:
    case EXPR_RELOP:
    case EXPR_UNOP:
        log_err("Attempted to evaluate a non-constant expr");
    }
    return -1;
}

struct StatementFor *statement_for_new(struct Expr *init, struct Expr *test, struct Expr *update, struct Statement *body) {
    struct StatementFor *loop = malloc(sizeof(struct StatementFor));
    check_mem(loop);
    loop->init = init;
    loop->test = test;
    loop->update = update;
    loop->body = body;
    return loop;
}

void statement_for_free(struct StatementFor *loop) {
    expr_free(loop->init);
    expr_free(loop->test);
    expr_free(loop->update);
    statement_free(loop->body);
    free(loop);
}

struct StatementJump *statement_jump_new(enum StatementJumpType type) {
    struct StatementJump *jump = malloc(sizeof(struct StatementJump));
    check_mem(jump);
    jump->type = type;
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

struct StatementLabeled *statement_labeled_new(enum StatementLabeledType type, void *label_or_test, struct Statement *statement) {
    struct StatementLabeled *labeled = malloc(sizeof(struct StatementLabeled));
    check_mem(labeled);
    labeled->type = type;
    labeled->label = label_or_test;
    labeled->statement = statement;
    return labeled;
}

void statement_labeled_free(struct StatementLabeled *labeled) {
    identifier_free(labeled->label);
    statement_free(labeled->statement);
    free(labeled);
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

struct StatementSwitch *statement_switch_new(struct Expr *test, struct Statement *body) {
    struct StatementSwitch *s = malloc(sizeof(struct StatementSwitch));
    check_mem(s);
    s->test = test;
    s->body = body;
    return s;
}

void statement_switch_free(struct StatementSwitch *s) {
    expr_free(s->test);
    statement_free(s->body);
    free(s);
}

struct StatementWhile *statement_while_new(struct Expr *test, struct Statement *body) {
    struct StatementWhile *loop = malloc(sizeof(struct StatementWhile));
    check_mem(loop);
    loop->test = test;
    loop->body = body;
    return loop;
}

void statement_while_free(struct StatementWhile *loop) {
    expr_free(loop->test);
    statement_free(loop->body);
    free(loop);
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
    case STMT_TYPE_FOR:
        statement_for_free(stmt->for_loop);
        break;
    case STMT_TYPE_RETURN:
    case STMT_TYPE_EXPR:
        expr_free(stmt->expr);
        break;
    case STMT_TYPE_JUMP:
        statement_jump_free(stmt->jump);
        break;
    case STMT_TYPE_LABELED:
        statement_labeled_free(stmt->labeled);
        break;
    case STMT_TYPE_SELECTION:
        statement_selection_free(stmt->selection);
        break;
    case STMT_TYPE_SWITCH:
        expr_free(stmt->switch_statement->test);
        statement_free(stmt->switch_statement->body);
        break;
    case STMT_TYPE_WHILE:
        statement_while_free(stmt->while_loop);
        break;
    }
    free(stmt);
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

void block_free(struct Block *block) {
    g_list_free_full(block->block_elements, (GDestroyNotify)block_element_free);
}

struct Function *function_new(struct Declarator *declarator, struct Block *body, GList *specifiers) {
    if (declarator->type != DECLARATOR_FUNCTION) {
        log_err("WARNING: attempting to declare a function with a declarator with the wrong type");
    }
    struct Function *function = malloc(sizeof(struct Function));
    check_mem(function);
    function->node_type = NODE_TYPE_FUNCTION;
    function->declarator = declarator;
    function->body = body;
    function->specifiers = specifiers;
    return function;
}

struct Identifier *function_identifier(struct Function *function) {
    return declarator_identifier(function->declarator);
}

void function_free(struct Function *func) {
    declarator_free(func->declarator);
    block_free(func->body);
    // TODO: free specifiers
    free(func);
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
