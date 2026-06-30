#ifndef COMPILER_H
#define COMPILER_H

#define SYMBOL_NAME_SIZE 64
#define SYMBOLS_TABLE_SIZE 256

#include "parser.h"
#include "vm/vm.h"

typedef struct Symbol {
	char name[SYMBOL_NAME_SIZE];
	size_t slot;
} Symbol;

typedef struct SymbolTable {
	Symbol symbols[SYMBOLS_TABLE_SIZE];
	size_t count;
} SymbolTable;

void compile(VM *vm, ASTNode *node);
void compile_node(VM *vm, ASTNode *node);

int lookup(SymbolTable *table, const char *name);
int add_var(SymbolTable *table, const char *name);

void compile_block(VM *vm, ASTNode *node);
void compile_literal(VM *vm, ASTNode *node);
void compile_identifier(VM *vm, ASTNode *node);

void compile_expr(VM *vm, ASTNode *node);
void compile_assignment(VM *vm, ASTNode *node);

void compile_print(VM *vm, ASTNode *node);
void compile_ifelse(VM *vm, ASTNode *node);

#endif