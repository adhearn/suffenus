#include <glib.h>
#include "util.h"
#include "lang.h"
#include "analysis.h"

static gboolean SYMBOL_TABLE_ERROR = FALSE;

void identifier_validate_declared(struct Identifier *identifier, struct SymbolTable *st, struct Identifier **id_loc) {
    struct Identifier *lookup = symbol_table_lookup(st, identifier->id);
    if (!lookup) {
        log_err("Undeclared variable: %s", identifier->id);
        SYMBOL_TABLE_ERROR = TRUE;
    } else if (lookup != identifier) {
        // We have two different identifier structs that correspond to the same identifier in the
        // source program. We need to update id_loc to point to the one in the symbol table,
        // then free the extra Identifier's memory.
        *id_loc = lookup;
        identifier_free(identifier);
    }
}

void expr_build_symbol_table(struct Expr *expr, struct SymbolTable *st) {
    switch (expr->type) {
    case (EXPR_ASSIGNMENT):
        identifier_validate_declared(expr->assignment->lhs, st, &expr->assignment->lhs);
        expr_build_symbol_table(expr->assignment->rhs, st);
        break;
    case (EXPR_CONSTANT):
        break;
    case (EXPR_IDENTIFIER):
        identifier_validate_declared(expr->id->id, st, &expr->id->id);
        break;
    case (EXPR_BINOP):
    case (EXPR_RELOP):
        expr_build_symbol_table(expr->op->arg1, st);
        expr_build_symbol_table(expr->op->arg2, st);
        break;
    }
}

void statement_jump_build_symbol_table(struct StatementJump *jump, struct SymbolTable *st) {
    switch(jump->type) {
    case JUMP_BREAK:
    case JUMP_CONTINUE:
    case JUMP_GOTO:
        break;
    case JUMP_RETURN:
        expr_build_symbol_table(jump->expr, st);
        break;
    }
}

void statement_build_symbol_table(struct Statement *statement, struct SymbolTable *st) {
    switch(statement->type) {
    case (STMT_TYPE_COMPOUND):
        block_build_symbol_table(statement->compound, st);
        break;
    case (STMT_TYPE_EXPR):
        expr_build_symbol_table(statement->expr, st);
        break;
    case (STMT_TYPE_JUMP):
        statement_jump_build_symbol_table(statement->jump, st);
        break;
    case (STMT_TYPE_SELECTION):
    {
        struct StatementSelection *selection = statement->selection;
        expr_build_symbol_table(selection->test, st);
        statement_build_symbol_table(selection->conseq, st);
        if (selection->alt) {
            statement_build_symbol_table(selection->conseq, st);
        }
        break;
    }
    default:
        log_err("Found unmatched statement type in statement_build_symbol_table: %d", statement->type);
        SYMBOL_TABLE_ERROR = TRUE;
    }
}

void declaration_build_symbol_table(struct Declaration *declaration, struct SymbolTable *st) {
    struct Type *type = declaration->type;
    struct Identifier *identifier = declaration->id;
    identifier->type = type;
    if (symbol_table_lookup_local(st, identifier->id)) {
        // TODO: Add line number information
        log_err("Variable already declared in scope: %s", identifier->id);

        // It's a little tricky to know what to do here - I'm assuming the behavior
        // is undefined. Let's not just give up, however. Instead, redeclare it and
        // continue. We probably need some way to bubble up an error.
        SYMBOL_TABLE_ERROR = TRUE;
    }
    symbol_table_extend(st, identifier);
}

void function_build_symbol_table(struct Function *func, struct SymbolTable *st) {
    // Functions are a somewhat complicated case. We need to add the function name to the symbol
    // table, then recur on the block that is the body, passing in the outer symbol table as
    // a parent.
    symbol_table_extend(st, func->name);
    struct Block *body = func->body;
    block_build_symbol_table(body, st);
}

void block_element_build_symbol_table(void *elt, void *untyped_st) {
    struct SymbolTable *st = (struct SymbolTable *)untyped_st;
    enum NodeType node_type = ast_node_type(elt);
    switch (node_type) {
    case NODE_TYPE_FUNCTION:
        function_build_symbol_table((struct Function *)elt, st);
        break;
    case NODE_TYPE_DECLARATION:
        declaration_build_symbol_table((struct Declaration *)elt, st);
        break;
    case NODE_TYPE_STATEMENT:
        statement_build_symbol_table((struct Statement*) elt, st);
        break;
    default:
        log_err("Received invalid node type in block_element_build_symbol_table: %d", node_type);
        SYMBOL_TABLE_ERROR = TRUE;
    }
}

void block_build_symbol_table(struct Block *block, struct SymbolTable *parent) {
    struct SymbolTable *st = symbol_table_new(parent);
    block->st = st;
    g_list_foreach(block->block_elements, (GFunc)block_element_build_symbol_table, (gpointer *)st);
}

gboolean build_symbol_table(struct Program *prog) {
    block_build_symbol_table(prog->top_level_block, NULL);
    return SYMBOL_TABLE_ERROR;
}
