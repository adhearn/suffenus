#include <stdlib.h>
#include "util.h"
#include "x64.h"

const char *ADDQ = "addq";
const char *IMUL = "imul";
const char *SUBQ = "subq";
const char *JE   = "je";
const char *JNE  = "jne";
const char *JG   = "jg";
const char *JL   = "jl";
const char *JGE  = "jge";
const char *JLE  = "jle";
const char *CMP  = "cmp";
const char *MOVQ = "movq";
const char *JMP  = "jmp";

struct x64_instruction *make_x64() {
    struct x64_instruction *x64 = malloc(sizeof(struct x64_instruction));
    check_mem(x64);

    return x64;
}
