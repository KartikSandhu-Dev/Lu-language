#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "vm.h"
#include "vm/bytecode.h"

size_t emit(VM *vm, OpCode opcode);
size_t emit_value(VM *vm, OpCode opcode, Value val);

void dump_compiled(VM *vm);

#endif