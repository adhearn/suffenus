#ifndef _tac_h
#define _tac_h

#include "list.h"

struct Program {
    List *statements;
};

struct Identifier {
    char *id;
};

struct Constant {
    int val;
};

struct Gensym {
    char *prefix;
    int val;
};

enum Op {
    Op_null,
    Op_or,
    Op_and,
    Op_add,
    Op_sub,
    Op_mul,
    Op_div,
    Op_mod,
    Op_lshift,
    Op_rshift,
    Op_eq,
    Op_neq,
    Op_lt,
    Op_gt,
    Op_lte,
    Op_gte,
    Op_jump
};

enum Address_type {
    Address_name,
    Address_constant,
    Address_gensym
};

struct Address {
    enum Address_type type;
    union {
        struct Identifier *name;
        struct Constant *constant;
        struct Gensym *gensym;
    } val;
};

struct Quad {
    enum Op op;
    struct Address *arg1;
    struct Address *arg2;
    void *result;
    struct Identifier *label;
};

struct Quad *make_Quad(void *result, enum Op op, struct Address *arg1, struct Address *arg2);
struct Identifier *make_Identifier(char *id);
struct Constant *make_Constant(int constant);
struct Address *make_Address_Identifier(struct Identifier *id);
struct Address *make_Address_Constant(struct Constant *id);
struct Program *make_Program(List *list);
void free_Program(struct Program *prog);

#endif
