#include "vm/stack.h"
#include "vm/bytecode.h"
#include <stdio.h>
#include <string.h>

void push(VM *vm, Value value) {
	if(vm->sp >= STACK_SIZE) {
		fprintf(stderr, "STACK OVERFLOW\n");
		exit(1);
	}
	vm->stack[vm->sp] = value;
	vm->sp++;
}

Value pop(VM *vm) {
	if(vm->sp <= 0) {
        fprintf(stderr, "STACK UNDERFLOW\n");
        exit(1);
    }
	vm->sp--;
	Value val = vm->stack[vm->sp];

	return val;
}

void store(VM *vm, size_t index) {
	vm->locals[index] = pop(vm);
}

void load(VM *vm, size_t index) {
	push(vm, vm->locals[index]);
}

void add(VM *vm) {
    Value rhs = pop(vm);
    Value lhs = pop(vm);

	if(lhs.type == INTEGER && rhs.type == INTEGER) {
		Value v3 = {0};
		v3.type = INTEGER;
		v3.int_value = lhs.int_value + rhs.int_value;
		push(vm, v3);
	} else {
		fprintf(stderr, "ADD needs 2 integers in the stack\n");
        exit(EXIT_FAILURE);
	}
}

void sub(VM *vm) {
    Value rhs = pop(vm);
    Value lhs = pop(vm);

	if(lhs.type == INTEGER && rhs.type == INTEGER) {
		Value v3 = {0};
		v3.type = INTEGER;
		v3.int_value = lhs.int_value - rhs.int_value;
		push(vm, v3);
	} else {
		fprintf(stderr, "SUB needs 2 integers in the stack\n");
        exit(EXIT_FAILURE);
	}
}

void mul(VM *vm) {
    Value rhs = pop(vm);
    Value lhs = pop(vm);

	if(lhs.type == INTEGER && rhs.type == INTEGER) {
		Value v3 = {0};
		v3.type = INTEGER;
		v3.int_value = lhs.int_value * rhs.int_value;
		push(vm, v3);
	} else {
		fprintf(stderr, "MUL needs 2 integers in the stack\n");
        exit(EXIT_FAILURE);
	}
}

void divi(VM *vm) {
    Value rhs = pop(vm);
    Value lhs = pop(vm);

	if(lhs.type == INTEGER && rhs.type == INTEGER) {
		Value v3 = {0};
		v3.type = INTEGER;
		v3.int_value = lhs.int_value / rhs.int_value;
		push(vm, v3);
	} else {
		fprintf(stderr, "DIV needs 2 integers in the stack\n");
        exit(EXIT_FAILURE);
	}
}

void eq(VM *vm) {
    Value rhs = pop(vm);
    Value lhs = pop(vm);

	if(lhs.type == INTEGER && rhs.type == INTEGER) {
		Value v3 = {0};
		v3.type = INTEGER;
		v3.int_value = lhs.int_value == rhs.int_value;
		push(vm, v3);
	} else if(lhs.type == STRING && rhs.type == STRING) {
		Value v3 = {0};
		v3.type = INTEGER;
		v3.int_value = strcmp(lhs.string_value, rhs.string_value);
		push(vm, v3);
	} else {
		fprintf(stderr, "EQ needs 2 integers in the stack\n");
        exit(EXIT_FAILURE);
	}
}

void gt(VM *vm) {
    Value rhs = pop(vm);
    Value lhs = pop(vm);

	if(lhs.type == INTEGER && rhs.type == INTEGER) {
		Value v3 = {0};
		v3.type = INTEGER;
		v3.int_value = lhs.int_value > rhs.int_value;
		push(vm, v3);
	} else {
		fprintf(stderr, "GT needs 2 integers in the stack\n");
        exit(EXIT_FAILURE);
    }
}

void lt(VM *vm) {
    Value rhs = pop(vm);
    Value lhs = pop(vm);

	if(lhs.type == INTEGER && rhs.type == INTEGER) {
		Value v3 = {0};
		v3.type = INTEGER;
		v3.int_value = lhs.int_value < rhs.int_value;
		push(vm, v3);
	} else {
		fprintf(stderr, "LT needs 2 integers in the stack\n");
        exit(EXIT_FAILURE);
	}
}


void print(VM *vm) {
	Value val = pop(vm);

	switch (val.type) {
        case INTEGER:
        	printf("%d\n", val.int_value);
        	break;
        case STRING:
        	printf("%s\n", val.string_value);
        	break;
    }
}

// Assembly uses 1-based line numbers.
// Convert to the VM's 0-based instruction index.

void jump(VM *vm, size_t target) {
	vm->ip = (target - 1);
}

void jump_if_false(VM *vm, size_t target) {
	Value cond = pop(vm);

	if (cond.type != INTEGER) {
        fprintf(stderr, "JUMP_IF_FALSE requires an integer condition\n");
        exit(EXIT_FAILURE);
    }

	if(cond.int_value == 0) {
		vm->ip = (target - 1);
	}
}