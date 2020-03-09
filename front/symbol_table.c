#include <glib.h>
#include "symbol_table.h"
#include "util.h"

struct SymbolTable *symbol_table_new(struct SymbolTable* parent) {
    struct SymbolTable *st = malloc(sizeof(struct SymbolTable));
    check_mem(st);

    st->parent = parent;
    GHashTable *table = g_hash_table_new_full(g_str_hash, g_str_equal, free, free_Identifier);
    st->table = table;
}

void symbol_table_free(SymbolTable *st) {
    g_hash_table_destry(st->table);
    free(st);
}

void symbol_table_extend(struct SymbolTable *st, struct Identifier *id) {
    if (g_hash_table_contains(st->table, key)) {
        log_error("Duplicate declaration for identifier: %s", id->id);
    } else {
        g_hash_table_(st->table, id->id, id);
    }
}

struct Identifier *symbol_table_lookup(struct SymbolTabler *st, char *key) {
    struct SymbolTable *cur = st;
    do {
        struct Identifier *match = (struct Identifier *)g_hash_table_lookup(st->table, key);
        if (match) {
            return match;
        }
    } while ((cur = cur->parent));
    return NULL;
}
