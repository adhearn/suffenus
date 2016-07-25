#include <stdlib.h>
#include <stdio.h>
#include "bb.h"
#include "tac.h"
#include "list.h"
#include "parser.tab.h"
#include "util.h"

void emit_asm_header() {
    printf(".text\n");
    printf(".globl main\n");
    printf("main:\n");
}

void emit_asm_footer() {
    printf("movq $60, %%rax\n");
    printf("syscall\n");
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
        log_err("Invalid address type: %d\n", addr->type);
    }
}

void emit_binop(struct Quad *stat) {
    switch (stat->op) {
    case Op_mul: // Different case, since it assumes that one of its operands is in RAX already
        printf("imul ");
        break;
    case Op_add:
        printf("addq ");
        break;
    case Op_sub:
        printf("subq ");
        break;
    default:
        log_err("Handling non-binop in emit_binop: %d\n", stat->op);
    }

    print_Address(stat->arg2);
    printf(", ");
    print_Address(stat->arg1);
    printf("\n");
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
        printf("jg");
        break;
    case Op_gte:
        printf("jge");
        break;
    case Op_lt:
        printf("jl");
        break;
    case Op_lte:
        printf("jle");
        break;
    default:
        log_err("Invalid operand %d", op);
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
    if (stat->label != NULL) {
        printf("%s: ", stat->label->id);
    }

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
        log_err("unable to handle op %d\n", stat->op);
    }
}

void emit_Block(struct BasicBlock *block) {
    struct InstructionNode *node = block->head;
    while (node != block->tail) {
        emit_Statement(node->instruction);
        node = node->next;
    }
    emit_Statement(node->instruction);
}

void emit_Program(struct Program *prog) {
    // Need to set up asm file
    emit_asm_header();

    List *statements = prog->statements;

    while (!List_empty(statements)) {
        struct BasicBlock *block = (struct BasicBlock *)List_pop(statements);
        emit_Block(block);
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
        log_err("Error while parsing\n");
        return 1;
    } else {
        List *blocks = buildBasicBlocks(prog);
        prog->statements = blocks;
        emit_Program(prog);
    }

    free_Program(prog);

    return 0;
}
