#ifndef BYTECODE_H
#define BYTECODE_H

#include <stdlib.h>

typedef enum OpCode {
	PUSH,
	POP,

	LOAD,
	STORE,

	ADD,
	SUB,
	MUL,
	DIV,

	EQ,
	GT,
	LT,

	PRINT,

	JUMP,
	JUMP_IF_FALSE,

	HALT,
} OpCode;

typedef enum ValueType {
	INTEGER,
	STRING,
} ValueType;

typedef struct Value {
	ValueType type;

	union {
		int int_value;
		char *string_value;
	};
	
} Value;

typedef struct Instruction {
	OpCode opcode;
	Value value;
} Instruction;

OpCode opcode_from_str(char *name);
size_t needs_input(OpCode opcode);

#endif