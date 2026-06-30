#ifndef VM_H
#define VM_H

#include "bytecode.h"
#include "parser.h"

#define STACK_SIZE 1024
#define PROGRAM_SIZE 1024
#define LOCAL_SIZE 1024

typedef struct VM {
	Instruction program[PROGRAM_SIZE];
	int code_size;

	Value stack[STACK_SIZE];
	int sp; // stack pointer

	Value locals[LOCAL_SIZE];

	int ip; // instruction pointer
} VM;

void vm_init(VM *vm, ASTNode *program_node);
void vm_run(VM *vm);

#endif