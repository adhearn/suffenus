#ifndef _instruction_selection_h
#define _instruction_selection_h

#include "list.h"

extern const char *ADDQ;
extern const char *IMUL;
extern const char *SUBQ;
extern const char *JE;
extern const char *JNE;
extern const char *JG;
extern const char *JL;
extern const char *JGE;
extern const char *JLE;
extern const char *CMP;
extern const char *MOVQ;
extern const char *JMP;

List *select_instructions(List *blocks);

#endif
