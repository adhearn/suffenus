#ifndef _bb_h
#define _bb_h

#include "tac.h"

struct InstructionNode {
    struct Quad *instruction;
    struct InstructionNode *next;
    struct InstructionNode *prev;
};

struct BasicBlock {
    struct InstructionNode *head;
    struct InstructionNode *tail;
};

struct BasicBlock *BasicBlock_new();
void BasicBlock_appendInstruction(struct BasicBlock *block, struct Quad *instruction);
List *buildBasicBlocks(struct Program *prog);

#endif
