#include <stdlib.h>
#include "bb.h"
#include "instruction_selection.h"
#include "list.h"
#include "util.h"
#include "x64.h"

void conditional_to_x64(struct Quad *quad, List *x64) {
    struct x64_instruction *cmp = make_x64();
    if (quad->label) {
        cmp->label= quad->label->label;
    } else {
        cmp->label = NULL;
    }

    cmp->arg1 = quad->arg1;
    cmp->arg2 = quad->arg2;
    cmp->op = CMP;
    List_append(x64, cmp);

    struct x64_instruction *jump = make_x64();
    switch(quad->op) {
    case Op_eq:
        jump->op = JE;
        break;
    case Op_neq:
        jump->op = JNE;
        break;
    case Op_gt:
        jump->op = JG;
        break;
    case Op_gte:
        jump->op = JGE;
        break;
    case Op_lte:
        jump->op = JLE;
        break;
    case Op_lt:
        jump->op = JL;
        break;
    default:
        log_err("Invalid conditional op: %d", quad->op);
        break;
    }

    jump->arg1 = quad->result;
    List_append(x64, jump);
}

void arithmetic_to_x64(struct Quad *quad, List *x64) {
    struct x64_instruction *instruction = make_x64();
    if (quad->label) {
        instruction->label = quad->label->label;
    } else {
        instruction->label = NULL;
    }
    instruction->arg1 = quad->arg1;
    instruction->arg2 = quad->arg2;

    switch (quad->op) {
    case Op_add:
        instruction->op = ADDQ;
        break;
    case Op_sub:
        instruction->op = SUBQ;
        break;
    case Op_mul:
        instruction->op = IMUL;
        break;
    case Op_or:
    case Op_and:
    case Op_div:
    case Op_mod:
    case Op_lshift:
    case Op_rshift:
        log_err("Arithmetic op not yet supported: %d", quad->op);
        break;
    default:
        log_err("Invalid arithmetic op: %d", quad->op);
        break;
    }

    List_append(x64, instruction);
}

void jump_to_x64(struct Quad *quad, List *x64) {
    struct x64_instruction *jump = make_x64();
    if (quad->label) {
        jump->label = quad->label->label;
    } else {
        jump->label = NULL;
    }

    jump->arg1 = quad->result;
    jump->op = JMP;

    List_append(x64, jump);
}

void assignment_to_x64(struct Quad *quad, List *x64) {
    struct x64_instruction *mov = make_x64();
    if (quad->label) {
        mov->label = quad->label->label;
    } else {
        quad->label = NULL;
    }

    mov->op = MOVQ;
    mov->arg1 = quad->result;
    mov->arg2 = quad->arg1;

    List_append(x64, mov);
}

/* Convert the instruction encoded as a TAC quad into one or more x64 instructions, and place
 * those instructions in the passed-in list */
void quad_to_x64(struct Quad *quad, List *x64) {
    switch (quad->op) {
    case Op_null:
        assignment_to_x64(quad, x64);
        break;
    case Op_eq:
    case Op_neq:
    case Op_gt:
    case Op_gte:
    case Op_lte:
    case Op_lt:
        conditional_to_x64(quad, x64);
        break;
    case Op_or:
    case Op_and:
    case Op_add:
    case Op_sub:
    case Op_mul:
    case Op_div:
    case Op_mod:
    case Op_lshift:
    case Op_rshift:
        arithmetic_to_x64(quad, x64);
        break;
    case Op_jump:
        jump_to_x64(quad, x64);
        break;
    default:
        log_err("Invalid op in quad_to_x64: %d", quad->op);
    }
}

struct BasicBlock *select_block_instructions(struct BasicBlock *block) {
    struct BasicBlock *newBlock = BasicBlock_new();
    struct InstructionNode *curNode = block->head;
    List *instructions = List_new(free);

    while(curNode != NULL) {
        struct Quad *quad = curNode->instruction.quad;
        quad_to_x64(quad, instructions);

        while (!List_empty(instructions)) {
            struct x64_instruction *x64 = List_pop(instructions);
            BasicBlock_append_x64_instruction(newBlock, x64);
        }

        curNode = curNode->next;
    }

    //List_free(instructions);
    return newBlock;
}

List *select_instructions(List *blocks) {
    List *selected = List_new(free);

    while(!List_empty(blocks)) {
        struct BasicBlock *block = List_pop(blocks);
        struct BasicBlock *newBlock = select_block_instructions(block);
        List_append(selected, newBlock);
    }

    return selected;
}
