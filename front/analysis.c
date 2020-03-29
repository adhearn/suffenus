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

void expr_assignment_build_symbol_table(struct ExprAssignment *assignment, struct SymbolTable *st) {
    switch (assignment->lvalue_type) {
    case LVALUE_IDENTIFIER: {
        identifier_validate_declared((struct Identifier *)assignment->lhs, st, (struct Identifier **)&assignment->lhs);
        break;
    }
    case LVALUE_POINTER:
        expr_build_symbol_table(((struct ExprOp *)assignment->lhs)->arg1, st);
        break;
    case LVALUE_INDEXED: {
        expr_indexed_build_symbol_table((struct ExprIndexed *)assignment->lhs, st);
        break;
    }
    }
    expr_build_symbol_table(assignment->rhs, st);
}

void expr_indexed_build_symbol_table(struct ExprIndexed *indexed, struct SymbolTable *st) {
    expr_build_symbol_table(indexed->expr, st);
    expr_build_symbol_table(indexed->index, st);
}

void expr_build_symbol_table(struct Expr *expr, struct SymbolTable *st) {
    switch (expr->type) {
    case (EXPR_ASSIGNMENT):
        expr_assignment_build_symbol_table(expr->assignment, st);
        break;
    case (EXPR_CALL):
        // The order in which these are evaluated is actually undefined in the standard. Becasue they're
        // exprs, though, they can't bind new variables, so we shouldn't have any issues with building
        // the symbol table.
        g_list_foreach(expr->call->args, (GFunc)expr_build_symbol_table, (gpointer)st);
        expr_build_symbol_table(expr->call->function, st);
        break;
    case (EXPR_CONSTANT):
        break;
    case (EXPR_IDENTIFIER):
        identifier_validate_declared(expr->id->id, st, &(expr->id->id));
        break;
    case (EXPR_INDEXED):
        expr_indexed_build_symbol_table(expr->indexed, st);
        break;
    case (EXPR_UNOP):
        expr_build_symbol_table(expr->op->arg1, st);
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
        break;
    case JUMP_GOTO:
        break;
    case JUMP_RETURN:
        expr_build_symbol_table(jump->expr, st);
        break;
    }
}

void statement_labeled_build_symbol_table(struct StatementLabeled *labeled, struct SymbolTable *st) {
    switch (labeled->type) {
    case LABELED_CASE:
        expr_build_symbol_table(labeled->test, st);
        break;
    case LABELED_DEFAULT:
        break;
    case LABELED_LABEL:
        log_err("LABELED_LABEL case not yet correctly implemented in analysis");
        break;
    default:
        log_err("Unknown StatementLabeled type received: %d", labeled->type);
    }
    statement_build_symbol_table(labeled->statement, st);
}

void statement_build_symbol_table(struct Statement *statement, struct SymbolTable *st) {
    switch(statement->type) {
    case (STMT_TYPE_COMPOUND): {
        struct SymbolTable *inner_st = symbol_table_new(st);
        block_build_symbol_table(statement->compound, inner_st);
        break;
    }
    case (STMT_TYPE_EXPR):
        expr_build_symbol_table(statement->expr, st);
        break;
    case (STMT_TYPE_FOR): {
        struct StatementFor *loop = statement->for_loop;
        expr_build_symbol_table(loop->init, st);
        expr_build_symbol_table(loop->test, st);
        if (loop->update) {
            expr_build_symbol_table(loop->update, st);
        }
        statement_build_symbol_table(loop->body, st);
        break;
    }
    case (STMT_TYPE_JUMP):
        statement_jump_build_symbol_table(statement->jump, st);
        break;
    case (STMT_TYPE_LABELED):
        // TODO: Need to add the label to the symbol table, so we can confirm that all gotos are to
        // actualy labels. However, we'll have to do that check after we've walked the whole program,
        // since labels can be jumped to before they actually show up.
        statement_labeled_build_symbol_table(statement->labeled, st);
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
    case (STMT_TYPE_SWITCH):
    {
        struct StatementSwitch *s = statement->switch_statement;
        expr_build_symbol_table(s->test, st);
        statement_build_symbol_table(s->body, st);
        break;
    }
    case (STMT_TYPE_WHILE): {
        struct StatementWhile *loop = statement->while_loop;
        expr_build_symbol_table(loop->test, st);
        statement_build_symbol_table(loop->body, st);
        break;
    }
    default:
        log_err("Found unmatched statement type in statement_build_symbol_table: %d", statement->type);
        SYMBOL_TABLE_ERROR = TRUE;
    }
}

// TODO: Add a type here
void declare_identifier(struct Identifier *identifier, struct SymbolTable *st) {
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

void declarator_build_symbol_table(struct Declarator *declarator, struct SymbolTable *st) {
    if (declarator->sub_declarator) {
        declarator_build_symbol_table(declarator->sub_declarator, st);
    }

    if (declarator->initializer) {
        expr_build_symbol_table(declarator->initializer, st);
    }

    switch(declarator->type) {
    case DECLARATOR_IDENTIFIER:
        declare_identifier(declarator->identifier, st);
        break;
    case DECLARATOR_ARRAY:
        if (declarator->array_size_expr) {
            expr_build_symbol_table(declarator->array_size_expr, st);
        }
        break;
    case DECLARATOR_FUNCTION:
        if (declarator->param_or_identifier_list) {
            // Not sure if this actually works or not...
            if (ast_node_type(declarator->param_or_identifier_list->data) == NODE_TYPE_IDENTIFIER) {
                g_list_foreach(declarator->param_or_identifier_list, (GFunc)declare_identifier, (gpointer *)st);
            } else {
                g_list_foreach(declarator->param_or_identifier_list, (GFunc)declaration_build_symbol_table, (gpointer *)st);
            }
        }
        break;
    }
}

void enum_build_symbol_table(struct Enum *enumeration, struct SymbolTable *st) {
    int next_value = 0;
    GList *enumerators = enumeration->enumerators;
    while (enumerators) {
        struct EnumElement *elt = enumerators->data;
        if (elt->expr) {
            next_value = expr_eval_constant(elt->expr);
        }
        elt->value = next_value;
        struct Constant *c = constant_new(next_value);
        elt->identifier->constant = c;
        symbol_table_extend(st, elt->identifier);
        next_value++;
        enumerators = enumerators->next;
    }
    return;
}

void specifiers_build_symbol_table(struct Type *type, struct SymbolTable *st) {
    switch (type->type) {
    case TYPE_ENUM:
        enum_build_symbol_table(type->enumeration, st);
        break;
    default:
        break;
    }
}

void declaration_build_symbol_table(struct Declaration *declaration, struct SymbolTable *st) {
    GList *specifiers = declaration->specifiers;
    g_list_foreach(specifiers, (GFunc)specifiers_build_symbol_table, (gpointer *)st);
    GList *declarators = declaration->declarators;
    g_list_foreach(declarators, (GFunc)declarator_build_symbol_table, (gpointer *)st);
}

void function_build_symbol_table(struct Function *func, struct SymbolTable *st) {
    declarator_build_symbol_table(func->declarator, st);
    struct SymbolTable *body_st = symbol_table_new(st);
    struct Block *body = func->body;
    block_build_symbol_table(body, body_st);
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

void block_build_symbol_table(struct Block *block, struct SymbolTable *st) {
    block->st = st;
    g_list_foreach(block->block_elements, (GFunc)block_element_build_symbol_table, (gpointer *)st);
}

gboolean build_symbol_table(struct Program *prog) {
    struct SymbolTable *st = symbol_table_new(NULL);
    block_build_symbol_table(prog->top_level_block, st);
    return SYMBOL_TABLE_ERROR;
}
