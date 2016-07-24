#include <stdlib.h>
#include <stdio.h>
#include "tac.h"
#include "list.h"
#include "parser.tab.h"

void emit_asm_header() {
    printf(".text\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("pushq %%rbp\n");
    printf("movq %%rsp, %%rbp\n");
}

void emit_asm_footer() {
    printf("popq %%rbp\n");
    printf("ret\n");
}

void print_Address(struct Address *addr) {
    switch (addr->type) {
    case (Address_gensym):
    case (Address_name):  // For now, identifiers must be register names
        printf("%%%s", addr->val.name->id);
        break;
    case (Address_constant):
        printf("$%d", addr->val.constant->val);
        break;
    default:
        printf("ERROR: Invalid address type: %d\n", addr->type);
    }
}

void emit_binop(struct Quad *stat) {
    switch (stat->op) {
    case Op_mul: // Different case, since it assumes that one of its operands is in RAX already
        printf("imul ");
        print_Address(stat->arg1);
        printf("\n");
        return; // Not a break
    case Op_add:
        printf("addq ");
        break;
    case Op_sub:
        printf("subq ");
        break;
    default:
        printf("ERROR: handling non-binop in emit_binop: %d\n", stat->op);
    }

    print_Address(stat->arg2);
    printf(", ");
    print_Address(stat->arg1);
}

void print_jump_instruction(enum Op op) {
    switch(op) {
    case Op_eq:
        printf("je");
        break;
    case Op_neq:
        printf("jne");
        break;
    case Op_gt:
        printf("gt");
        break;
    case Op_gte:
        printf("gte");
        break;
    case Op_lt:
        printf("lt");
        break;
    case Op_lte:
        printf("lte");
        break;
    }
}

void emit_conditional(struct Quad *stat) {
    printf("cmp ");
    print_Address(stat->arg2);
    printf(", ");
    print_Address(stat->arg1);
    printf("\n");
    print_jump_instruction(stat->op);
    printf(" %s\n", ((struct Identifier *)stat->result)->id);;
}

void emit_Statement(struct Quad *stat) {
    switch (stat->op) {
    case Op_null:
        printf("movq ");
        print_Address(stat->arg1);
        printf(", ");
        print_Address((struct Address *)stat->result);
        printf("\n");
        break;
    case Op_jump:
        printf("jmp %s\n", ((struct Identifier *)stat->result)->id);
        break;
    case Op_add:
    case Op_sub:
    case Op_mul:
        emit_binop(stat);
        break;
    case Op_eq:
    case Op_neq:
    case Op_lt:
    case Op_gt:
    case Op_gte:
    case Op_lte:
        emit_conditional(stat);
        break;
    default:
        printf("ERROR: unable to handle op %d\n", stat->op);
    }
}

void emit_Program(struct Program *prog) {
    // Need to set up asm file
    emit_asm_header();

    List *statements = prog->statements;

    while (!List_empty(statements)) {
        struct Quad *quad = (struct Quad *)List_pop(statements);
        emit_Statement(quad);
    }

    // emit rest of file
    emit_asm_footer();
}

int main(int argc, char *argv[]) {
    extern int yydebug;
    yydebug = 0;
    struct Program *prog = (struct Program *)malloc(sizeof(struct Program));
    int res = yyparse(&prog);

    if (res != 0) {
        fprintf(stderr, "Error while parsing\n");
        return 1;
    } else {
        printf("Parsed %d statements\n", List_size(prog->statements));
        emit_Program(prog);
    }

    free_Program(prog);

    return 0;
}
