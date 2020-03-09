#include <stdlib.h>
#include <stdio.h>
#include "lang.h"
#include "parser.tab.h"
#include "util.h"


int main(int argc, char *argv[]) {
    extern int yydebug;
    yydebug = 0;
    struct Program *prog = (struct Program *)malloc(sizeof(struct Program));
    check_mem(prog);
    int res = yyparse(&prog);

    if (res != 0) {
        log_err("Error while parsing\n");
        return 1;
    } else {
        print_Program(prog);
    }

    free_Program(prog);
    printf("Done, exiting\n");

    return 0;
}
