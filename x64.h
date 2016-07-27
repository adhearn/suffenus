#ifndef _x64_h
#define _x64_h

struct x64_instruction {
    const char *label;
    const char *op;
    struct Address *arg1; // For now, just use Addresses.
    struct Address *arg2; // We'll need something else for register allocation
};

struct x64_instruction *make_x64();

#endif
