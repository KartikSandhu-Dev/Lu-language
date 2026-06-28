#include "vm/bytecode.h"
#include <string.h>

OpCode opcode_from_str(char *name) {
    if (strcmp(name, "PUSH") == 0) return PUSH;
    if (strcmp(name, "POP") == 0) return POP;

    if (strcmp(name, "LOAD") == 0) return LOAD;
    if (strcmp(name, "STORE") == 0) return STORE;

    if (strcmp(name, "ADD") == 0) return ADD;
    if (strcmp(name, "SUB") == 0) return SUB;
    if (strcmp(name, "MUL") == 0) return MUL;
    if (strcmp(name, "DIV") == 0) return DIV;

    if (strcmp(name, "EQ") == 0) return EQ;
    if (strcmp(name, "GT") == 0) return GT;
    if (strcmp(name, "LT") == 0) return LT;

    if (strcmp(name, "JUMP") == 0) return JUMP;
    if (strcmp(name, "JUMP_IF_FALSE") == 0) return JUMP_IF_FALSE;

    if (strcmp(name, "PRINT") == 0) return PRINT;
    if (strcmp(name, "HALT") == 0) return HALT;

    return -1;
}

size_t needs_input(OpCode opcode) {
    if (opcode == PUSH) return 1;

    if (opcode == LOAD) return 1;
    if (opcode == STORE) return 1;

    if (opcode == JUMP) return 1;
    if (opcode == JUMP_IF_FALSE) return 1;

    return 0;
}