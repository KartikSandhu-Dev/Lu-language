#include "../header/parser.h"
#include "../header/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ASTNode *program_node;

// returns ptr to current element of parser
Token *current(Parser *p) {
	return &p->tokens[p->pos];
}

// advance to the next element
void advance(Parser *p) {
	p->pos++;
}

// harcore expect
static void expect(Parser *p, TokenType type) {
	if(current(p)->type != type) {
		fprintf(stderr, "Unexpected Token\n");
		exit(1);
	}
	advance(p);
}

// PARENT NODE
ASTNode *parse_program(Parser *p) {
	ASTNode *progNode = malloc(sizeof(ASTNode));
	progNode->program.capacity = num_of_tokens();
	progNode->program.count = 0;

	progNode->program.statements = malloc(sizeof(ASTNode)*progNode->program.capacity);

	while(current(p)->type != TOKEN_EOF) {
		progNode->program.statements[progNode->program.count] = parse_statement(p);

		progNode->program.count++;

		if(progNode->program.count >= progNode->program.capacity) {
			progNode->program.statements = realloc(progNode->program.statements, progNode->program.count + 10);
		}
	}

	if(progNode == NULL) { free(progNode); }
	else {
		program_node = progNode;
		return progNode;
	}

	return NULL;
}

ASTNode *parse_statement(Parser *p) {

	if(current(p)->type == TOKEN_EOL || (current(p)->type == TOKEN_KEYWORD && 
		strcmp(current(p)->value, "end") == 0)){

		advance(p);

	}
	return NULL;
}

ASTNode *parse_assignment(Parser *p) {
	if(current(p)->type == TOKEN_IDENTIFIER) {
		ASTNode *id = malloc(sizeof(ASTNode));
		id->type = NODE_IDENTIFIER;
		id->value = current(p)->value;


		expect(p, TOKEN_ASSINGMENT);

		ASTNode *node = malloc(sizeof(ASTNode));
		node->type = NODE_ASSINGMENT;
		node->value = NULL;

		advance(p);

		ASTNode *expr = parse_expression(p);

		if(expr == NULL) {
			fprintf(stderr, "Expected expression after '='\n");
 			exit(1);
		}

		node->assignment.target = id;
		node->assignment.value = expr;

		return node;

	}	
	return NULL;
}

ASTNode *parse_expression(Parser *p) {
	ASTNode *node = malloc(sizeof(ASTNode));

	if(current(p)->type == TOKEN_LPARANTHES) {
		advance(p);
		if(current(p)->type == TOKEN_OPERATOR) {
			if(strcmp(current(p)->value, "+")) {

			}
		}
	} else if(current(p)->type == TOKEN_STRING) {
		ASTNode *node = malloc(sizeof(ASTNode));

		return node;
	} else if(current(p)->type == TOKEN_INT) {

	} else if(current(p)->type == TOKEN_IDENTIFIER) {

	}

	if(node == NULL) { free(node); }

	return NULL;
}

ASTNode *parse_print(Parser *p) {
	if(current(p)->type == TOKEN_KEYWORD) {
		advance(p);
		if(current(p)->type == TOKEN_IDENTIFIER || current(p)->type == TOKEN_STRING || 
		   current(p)->type == TOKEN_INT) {

		   	ASTNode *node = malloc(sizeof(ASTNode));
			node->type = NODE_PRINT;
			node->value = NULL;

			ASTNode *expr = parse_expression(p);

			if(expr == NULL) {
				fprintf(stderr, "Nothing to print\n");
				exit(1);
			}

			node->print.expr = expr;

			return node;
		}
	}
	return NULL;
}

void clean_ASTs() {

}