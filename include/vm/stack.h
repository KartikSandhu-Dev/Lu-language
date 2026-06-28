#ifndef STACK_H
#define STACK_H

#include "vm.h"
#include "bytecode.h"
#include <stdlib.h>

void push(VM *vm, Value value);
Value pop(VM *vm);

void store(VM *vm, size_t index);
void load(VM *vm, size_t index);

void add(VM *vm);
void sub(VM *vm);
void mul(VM *vm);
void divi(VM *vm);

void eq(VM *vm);
void gt(VM *vm);
void lt(VM *vm);

void jump(VM *vm, size_t target);
void jump_if_false(VM *vm, size_t target);

void print(VM *vm);

#endif