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

struct Label {
    char *label;
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
    Address_label,
    Address_constant,
    Address_gensym
};

struct Address {
    enum Address_type type;
    union {
        struct Label *label;
        struct Identifier *name;
        struct Constant *constant;
        struct Gensym *gensym;
    } val;
};

struct Quad {
    enum Op op;
    struct Address *arg1;
    struct Address *arg2;
    struct Address *result;
    struct Label *label;
};

struct Quad *make_Quad(void *result, enum Op op, struct Address *arg1, struct Address *arg2);
struct Identifier *make_Identifier(char *id);
struct Constant *make_Constant(int constant);
struct Label *make_Label(char *label);
struct Address *make_Address_Identifier(struct Identifier *id);
struct Address *make_Address_Constant(struct Constant *id);
struct Address *make_Address_Label(struct Label *label);
struct Program *make_Program(List *list);
void free_Program(struct Program *prog);
void tac_print_Quad(struct Quad *quad);
void tac_print_Address(struct Address *addr);

#endif
