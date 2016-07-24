#include <stdlib.h>
#include "tac.h"
#include "util.h"

struct Program *make_Program(List *list) {
    struct Program *prog = malloc(sizeof(struct Program));
    check_mem(prog);

    prog->statements = list;
    return prog;
}

struct Identifier *make_Identifier(char *id) {
    struct Identifier *ident = malloc(sizeof(struct Identifier));
    check_mem(id);

    ident->id = id;
    return ident;
}

struct Constant *make_Constant(int val) {
    struct Constant *c = malloc(sizeof(struct Identifier));
    check_mem(c);

    c->val = val;
    return c;
}

struct Quad *make_Quad(void *result, enum Op op, struct Address *arg1, struct Address *arg2) {
    struct Quad *quad = malloc(sizeof(struct Quad));
    check_mem(quad);

    quad->result = result;
    quad->op = op;
    quad->arg1 = arg1;
    quad->arg2 = arg2;
    quad->label = NULL;

    return quad;
}

struct Address *make_Address_Identifier(struct Identifier *id) {
    struct Address *addr = malloc(sizeof(struct Quad));
    check_mem(addr);

    addr->type = Address_name;
    addr->val.name = id;

    return addr;
}

struct Address *make_Address_Constant(struct Constant *constant) {
    struct Address *addr = malloc(sizeof(struct Quad));
    check_mem(addr);

    addr->type = Address_constant;
    addr->val.constant = constant;

    return addr;
}

void free_Program(struct Program *prog) {
    // free
}
