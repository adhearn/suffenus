#ifndef _symbol_table_h
#define _symbol_table_h

#include <glib.h>
#include <lang.h>

struct SymbolTable {
    struct SymbolTable *parent;
    GHashTable *table;
};

struct SymbolTable *symbol_table_new(struct SymbolTable* parent);
void symbol_table_free(SymbolTable *st);
void symbol_table_extend(char *key, struct Identifier *id);
struct Identifier *symbol_table_lookup(char *key);

#endif
