#ifndef _analysis_h
#define _analysis_h

#include <glib.h>
#include "lang.h"

void identifier_validate_declared(struct Identifier *identifier, struct SymbolTable *st, struct Identifier **id_loc);
void expr_build_symbol_table(struct Expr *expr, struct SymbolTable *st);

void statement_build_symbol_table(struct Statement* statement, struct SymbolTable *st);
void declaration_build_symbol_table(struct Declaration *declaration, struct SymbolTable *st);
void function_build_symbol_table(struct Function *func, struct SymbolTable *st);
void block_element_build_symbol_table(void *elt, void *st);
void block_build_symbol_table(struct Block *block, struct SymbolTable *parent);
gboolean build_symbol_table(struct Program *prog);

#endif
