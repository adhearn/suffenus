#include <stdlib.h>
#include <stdio.h>
#include "lang.h"
#include "list.h"
#include "parser.tab.h"

int main(int argc, char *argv[]) {
    extern yydebug;
    yydebug = 0;
    int res = yyparse();

    if (res != 0) {
        fprintf(stderr, "Error while parsing\n");
    } else {
        printf("Parse successful\n");
    }

    return 0;
}
