#include "vm/instruction.h"
#include "vm/bytecode.h"
#include <stdio.h>

size_t emit(VM *vm, OpCode opcode) {
	size_t index = vm->code_size;

	Instruction inst = {0};
	inst.opcode = opcode;

	vm->program[vm->code_size] = inst;
	vm->code_size++;

	return index;
}

size_t emit_value(VM *vm, OpCode opcode, Value val) {
	size_t index = vm->code_size;

	Instruction inst = {0};
	inst.opcode = opcode;
	inst.value = val;

	vm->program[vm->code_size] = inst;
	vm->code_size++;

	return index;
}

void dump_compiled(VM *vm) {
	FILE *file = fopen("prac.lub", "w");

	if(file == NULL) { return; }

	size_t pos = 0;

	while(1) {
		OpCode opcode = vm->program[pos].opcode;
		Value val = vm->program[pos].value;

		if(val.type == INTEGER) {
			fprintf(file, "%s %d\n", opcode_to_string(opcode), val.int_value);
		} else if(val.type == STRING) {
			fprintf(file, "%s %s\n", opcode_to_string(opcode), val.string_value);
		} else {
			fprintf(file, "%s\n", opcode_to_string(opcode));
		}

		if(vm->program[pos].opcode == HALT) {
			break;
		}

		pos++;
	}
}