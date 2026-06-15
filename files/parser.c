#include "../header/parser.h"
#include "../header/lexer.h"
#include <string.h>

// returns ptr to current element of parser
Token *current(Parser *p) {
	return &p->tokens[p->pos];
}

// advance to the next element
void advance(Parser *p) {
	p->pos++;
}

// go back to the previous element
void previous(Parser *p) {
	p->pos--;
}

void parse_program(Parser *p) {
	while(current(p)->type != TOKEN_EOF) {
		parse_statement(p);
	}
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
		advance(p);
		if(current(p)->type == TOKEN_OPERATOR && (strcmp(current(p)->value, "=") == 0)) {	
			ASTNode *node = malloc(sizeof(ASTNode));
			node->type = NODE_ASSINGMENT;
			node->value = current(p)->value;

			previous(p); //before the operator

			ASTNode *id = malloc(sizeof(ASTNode));
			id->type = NODE_IDENTIFIER;

			ASTNode *expr = malloc(sizeof(ASTNode));
			
			while(current(p)->type != TOKEN_EOL) {
				if(current(p)->type == TOKEN_IDENTIFIER) {
					id->value = current(p)->value;

				} else if(current(p)->type == TOKEN_STRING) {
					expr->value = current(p)->value;
					expr->type = NODE_STRING;

				} else if(current(p)->type == TOKEN_INT) {
					
				}


				advance(p);
			}

			node->assignment.target = id;
			node->assignment.value = expr;

		} else {
			exit(1);
		}
	}	
	return NULL;
}

ASTNode *parse_expression(Parser *p) {
	if(current(p)->type == TOKEN_INT || (strcmp(current(p)->value, "("))) {
		advance(p);
		if(current(p)->type == TOKEN_OPERATOR) {
			if(strcmp(current(p)->value, "+")) {

			}
		}
	} else if(current(p)->type == TOKEN_STRING) {
		ASTNode *node = malloc(sizeof(ASTNode));

		return node;
	}
	return NULL;
}

ASTNode *parse_print(Parser *p) {
	if(current(p)->type == TOKEN_KEYWORD) {
		advance(p);
		if(current(p)->type == TOKEN_IDENTIFIER || current(p)->type == TOKEN_STRING || 
		   current(p)->type == TOKEN_INT) {

		   	ASTNode *node = malloc(sizeof(ASTNode));
			node->type = NODE_PRINT;

		}
	}
	return NULL;
}

void clean_ASTs() {

}