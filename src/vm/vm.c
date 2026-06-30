#include "vm/vm.h"
#include "parser.h"
#include "vm/instruction.h"
#include "vm/stack.h"
#include "compiler.h"

void vm_init(VM *vm, ASTNode *program_node) {
	vm->code_size = 0;
	vm->sp = 0;
	vm->ip = 0;

	compile(vm, program_node);
	dump_compiled(vm);
}

void vm_run(VM *vm) {
	while(vm->ip < vm->code_size) {
		Instruction inst = vm->program[vm->ip];

		vm->ip++;

		switch (inst.opcode) {
	        case PUSH:
	        	push(vm, inst.value);
	        	break;

	        case POP:
	        	pop(vm);
	        	break;

	        case LOAD:
	        	load(vm, inst.value.int_value);
	        	break;

	        case STORE:
	        	store(vm, inst.value.int_value);
	        	break;

	        case ADD:
	        	add(vm);
	        	break;

	        case SUB:
	        	sub(vm);
	        	break;

	        case MUL:
	        	mul(vm);
	        	break;

	        case DIV:
	        	divi(vm);
	        	break;

	        case EQ:
	        	eq(vm);
	        	break;

	        case GT:
	        	gt(vm);
	        	break;

	        case LT:
	        	lt(vm);
	        	break;

	        case PRINT:
	        	print(vm);
	        	break;

	        case JUMP:
	        	jump(vm, inst.value.int_value);
	        	break;

	        case JUMP_IF_FALSE:
	        	jump_if_false(vm, inst.value.int_value);
	        	break;

	        case HALT:
	        	return;
	        	
		}
    }
}