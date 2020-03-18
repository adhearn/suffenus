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
    printf("Space alloated for program, parsing\n");
    int res = yyparse(&prog);
    if (res != 0) {
        log_err("Error while parsing\n");
        return 1;
    } else {
        program_print(prog);
    }

    printf("==========\n");
    printf("Beginning analysis pass...\n");
    printf("Building symbol table\n");
    build_symbol_table(prog);
    printf("Symbol table built\n");

    printf("==========\n");
    GList *quads = program_generate_tac_quads(prog);
    g_list_foreach(quads, (gpointer)quad_print, NULL);
    printf("==========\n");

    printf("Freeing program...\n");

    program_free(prog);
    printf("Done, exiting\n");

    return 0;
}
