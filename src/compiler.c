#include "compiler.h"
#include "parser.h"
#include "vm/bytecode.h"
#include "vm/instruction.h"
#include <stdio.h>
#include <string.h>

SymbolTable symbol_table = {0};

void compile(VM *vm, ASTNode *node)
{
    symbol_table.count = 0;

    compile_node(vm, node);

    emit(vm, HALT);
}

void compile_node(VM *vm, ASTNode *node) {
	if(!node) { return; }

	switch(node->type) {

	    case NODE_PROGRAM:
	    	compile_block(vm, node);
	    	break;
	    	
	    case NODE_BLOCK:
	    	compile_block(vm, node);
	    	break;

	    case NODE_INT:
	    	compile_literal(vm, node);
	    	break;

	    case NODE_STRING:
	    	compile_literal(vm, node);
	    	break;

	    case NODE_IDENTIFIER:
	    	compile_identifier(vm, node);
	    	break;

	    case NODE_PLUS:
	    	compile_expr(vm, node);
	    	break;

	    case NODE_MULTIPLY:
	    	compile_expr(vm, node);
	    	break;

	    case NODE_DIVIDE:
	    	compile_expr(vm, node);
	    	break;

	    case NODE_MINUS:
	    	compile_expr(vm, node);
	    	break;

	    case NODE_EQUALS:
	    	compile_expr(vm, node);
	    	break;

	    case NODE_GREATTHAN:
	    	compile_expr(vm, node);
	    	break;

	    case NODE_LESSTHAN:
	    	compile_expr(vm, node);
	    	break;

	    case NODE_NOTEQUALS:
	    	compile_expr(vm, node);
	    	break;

	    case NODE_ASSIGNMENT:
	    	compile_assignment(vm, node);
	    	break;

	    case NODE_PRINT:
	    	compile_print(vm, node);
	    	break;

	    case NODE_IF:
	    	compile_ifelse(vm, node);
	    	break;
	}

}

Value make_int(int val) {
	Value v = {0};
	v.type = INTEGER;
	v.int_value = val;
	return v;
}

void compile_block(VM *vm, ASTNode *node) {
	if(!node) { return; }

	size_t pos = 0;
	while(node->program.statements[pos] != NULL) {
		compile_node(vm, node->program.statements[pos]);
		pos++;
	}
}

void compile_literal(VM *vm, ASTNode *node) {
	Value val = {0};
	if(node->type == NODE_INT) {
		val.type = INTEGER;
		val.int_value = node->int_value;

	} else if(node->type == NODE_STRING) {
		val.type = STRING;
		val.string_value = node->string_value;
	} 

	emit_value(vm, PUSH, val);
}

void compile_identifier(VM *vm, ASTNode *node) {
	if(node->type == NODE_IDENTIFIER) {
		int slot = lookup(&symbol_table, node->identifier_value);

		if(slot == -1) {
			fprintf(stderr, "Unknown variable used %s\n", node->identifier_value);
		}

		Value val = {0};
		val.type = INTEGER;
		val.int_value = slot;

		emit_value(vm, LOAD, val);

	} else {
		fprintf(stderr, "Not an identifier.\n");
		exit(1);
	}
}

void compile_assignment(VM *vm, ASTNode *node) {
	compile_node(vm, node->assignment.value);

	int slot = lookup(&symbol_table, node->assignment.target->identifier_value);

	if(slot == -1) {
		slot = add_var(&symbol_table, node->assignment.target->identifier_value);
	}

	Value val = make_int(slot);

	emit_value(vm, STORE, val);

}

void compile_expr(VM *vm, ASTNode *node) {
	compile_node(vm, node->expression.left);
	compile_node(vm, node->expression.right);

	switch (node->type) {
        case NODE_PLUS:
        	emit(vm, ADD);
        	break;

        case NODE_MULTIPLY:
        	emit(vm, MUL);
        	break;

        case NODE_DIVIDE:
        	emit(vm, DIV);
        	break;

        case NODE_MINUS:
        	emit(vm, SUB);
        	break;

        case NODE_EQUALS:
        	emit(vm, EQ);
        	break;

        case NODE_GREATTHAN:
        	emit(vm, GT);
        	break;

        case NODE_LESSTHAN:
        	emit(vm, LT);
        	break;

       	default:
       		fprintf(stderr, "Bad syntax\n");
       		exit(1);
       		break;
    }
}

void compile_print(VM *vm, ASTNode *node) {
	compile_node(vm, node->print.expr);

	emit(vm, PRINT);
}

void patch_jump(VM *vm, size_t instruction) {
	vm->program[instruction].value.int_value = vm->code_size + 1;
}

void compile_ifelse(VM *vm, ASTNode *node) {
	compile_expr(vm, node->ifelse.condition);

	Value val_false = make_int(0);
	size_t instruction_false = emit_value(vm, JUMP_IF_FALSE, val_false);

	compile_block(vm, node->ifelse.ifBody);

	patch_jump(vm, instruction_false);

	Value val_true = make_int(0);
	size_t instruction_true = emit_value(vm, JUMP, val_true);

	compile_block(vm, node->ifelse.elseBody);
	patch_jump(vm, instruction_true);

}

int add_var(SymbolTable *table, const char *name) {
	int slot = table->count;

	if(lookup(table, name) != -1) { 
		fprintf(stderr, "Adding already used variable to the SymbolTable %s\n", name); 
		exit(1);
	}

	strcpy(table->symbols[slot].name, name);
	table->symbols[slot].slot = slot;

	table->count++;

	return slot;
}

int lookup(SymbolTable *table, const char *name) {
	for(size_t i = 0; i < table->count; i++) {
		if(strcmp(table->symbols[i].name, name) == 0) {
			return table->symbols[i].slot;
		}
	}
	return -1;
}

