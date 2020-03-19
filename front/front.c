#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "lang.h"
#include "analysis.h"
#include "icg.h"
#include "parser.tab.h"

int main(int argc, char *argv[]) {
    extern int yydebug;
    yydebug = 0;
    struct Program *prog = (struct Program *)malloc(sizeof(struct Program));
    check_mem(prog);
    int res = yyparse(&prog);
    if (res != 0) {
        log_err("Error while parsing\n");
        return 1;
    }

    build_symbol_table(prog);
    GList *quads = program_generate_tac_quads(prog);
    g_list_foreach(quads, (gpointer)quad_print, NULL);
    // program_free(prog);
    return 0;
}
