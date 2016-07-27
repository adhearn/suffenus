#include <stdlib.h>
#include <stdio.h>
#include "bb.h"
#include "tac.h"
#include "util.h"

int end_of_block(struct Quad *instruction) {
    switch (instruction->op) {
    case Op_jump:
    case Op_gt:
    case Op_lt:
    case Op_gte:
    case Op_lte:
    case Op_eq:
    case Op_neq:
        return 1;
    default:
        return 0;
    }
}

List *buildBasicBlocks(struct Program *prog) {
    List *ls = prog->statements;
    List *blocks = List_new(free);

    struct BasicBlock *curBlock = BasicBlock_new();
    List_append(blocks, curBlock);
    int endOfBlock = 0;

    while (!List_empty(ls)) {
        struct Quad *quad = List_pop(ls);

        if (endOfBlock) {
            if (curBlock->head) { // Don't create a new block if there's nothing in the old block
                curBlock = BasicBlock_new();
                List_append(blocks, curBlock);
            }
            endOfBlock = 0;
        } else if (quad->label != NULL) { // start a new block
            if (curBlock->head) { // Don't create a new block if there's nothing in the old block
                curBlock = BasicBlock_new();
                List_append(blocks, curBlock);
            }
        }

        if (end_of_block(quad)) {
            endOfBlock = 1;
        }

        BasicBlock_append_Quad(curBlock, quad);
    }

    return blocks;
}

struct BasicBlock *BasicBlock_new() {
    struct BasicBlock *block = malloc(sizeof(struct BasicBlock));
    check_mem(block);

    block->head = NULL;
    block->tail = NULL;

    return block;
}

void BasicBlock_appendInstruction(struct BasicBlock *block, union Instruction instruction) {
    struct InstructionNode *node = malloc(sizeof(struct InstructionNode));
    node->instruction = instruction;
    node->next = NULL;
    node->prev = block->tail;

    if (block->head == NULL) {
        block->head = node;
        block->tail = node;
    } else if (block->tail != NULL) {
        block->tail->next = node;
    }

    block->tail = node;
}

void BasicBlock_append_Quad(struct BasicBlock *block, struct Quad *quad) {
    union Instruction instruction = { quad: quad };
    BasicBlock_appendInstruction(block, instruction);
}

void BasicBlock_append_x64_instruction(struct BasicBlock *block, struct x64_instruction *x64) {
    union Instruction instruction = { x64: x64 };
    BasicBlock_appendInstruction(block, instruction);
}
