#ifndef _bb_h
#define _bb_h

#include "tac.h"
#include "x64.h"

union Instruction{
    struct Quad *quad;
    struct x64_instruction *x64;
};

struct InstructionNode {
    union Instruction instruction;
    struct InstructionNode *next;
    struct InstructionNode *prev;
};

struct BasicBlock {
    struct InstructionNode *head;
    struct InstructionNode *tail;
};

struct BasicBlock *BasicBlock_new();
void BasicBlock_append_Quad(struct BasicBlock *block, struct Quad *quad);
void BasicBlock_append_x64_instruction(struct BasicBlock *block, struct x64_instruction *x64);
List *buildBasicBlocks(struct Program *prog);
void BasicBlock_free(struct BasicBlock *block);

#endif
