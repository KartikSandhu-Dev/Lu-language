#include "vm/bytecode.h"
#include <string.h>

char *opcode_to_string(OpCode opcode) {
    switch (opcode) {
        case PUSH:
            return "PUSH";
            break;
        case POP:
            return "POP";
            break;
        case LOAD:
            return "LOAD";
            break;
        case STORE:
            return "STORE";
            break;
        case ADD:
            return "ADD";
            break;
        case SUB:
            return "SUB";
            break;
        case MUL:
            return "MUL";
            break;
        case DIV:
            return "DIV";
            break;
        case EQ:
            return "EQ";
            break;
        case GT:
            return "GT";
            break;
        case LT:
            return "LT";
            break;
        case PRINT:
            return "PRINT";
            break;
        case JUMP:
            return "JUMP";
            break;
        case JUMP_IF_FALSE:
            return "JUMP_IF_FALSE";
            break;
        case HALT:
            return "HALT";
            break;
    }
    return "NULL";
}

size_t needs_input(OpCode opcode) {
    if (opcode == PUSH) return 1;

    if (opcode == LOAD) return 1;
    if (opcode == STORE) return 1;

    if (opcode == JUMP) return 1;
    if (opcode == JUMP_IF_FALSE) return 1;

    return 0;
}