#include "../header/parser.h"
#include "../header/lexer.h"
#include <exception>
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

void previous(Parser *p) {
	p->pos--;
}

// harcore expect
static void expect(Parser *p, TokenType type) {
	if(current(p)->type != type) {
		fprintf(stderr, "Unexpected Token.\n");
		exit(1);
	}
	advance(p);
}

// PARENT NODE
ASTNode *parse_program(Parser *p) {
	ASTNode *progNode = malloc(sizeof(ASTNode));

	size_t *capacity = &progNode->program.capacity;
	size_t *count = &progNode->program.count;

	*capacity = num_of_tokens();
	*count = 0;

	progNode->program.statements = malloc(sizeof(ASTNode*)*(*capacity));

	while(current(p)->type != TOKEN_EOF) {
		progNode->program.statements[*count] = parse_statement(p);
		
		(*count)++;

		if(*count >= *capacity) {
			*capacity = *count + 10;
			progNode->program.statements = 
					realloc(progNode->program.statements, 
						sizeof(ASTNode*)*(*capacity));
		}
	}

	if(current(p)->type == EOF) {
		progNode->program.statements[*count] = NULL;
	}

	program_node = progNode;
	return progNode;
}

ASTNode *parse_statement(Parser *p) {
	if(current(p)->type == TOKEN_EOL || (current(p)->type == TOKEN_KEYWORD && 
		(strcmp(current(p)->value, "end") == 0))){
		advance(p);
	}

	parse_assignment(p);
	parse_print(p);

	return NULL;
}

ASTNode *parse_assignment(Parser *p) {
	if(current(p)->type == TOKEN_IDENTIFIER) {
		ASTNode *id = malloc(sizeof(ASTNode));
		id->type = NODE_IDENTIFIER;
		id->value = current(p)->value;

		advance(p);
		expect(p, TOKEN_ASSINGMENT);

		ASTNode *node = malloc(sizeof(ASTNode));
		node->type = NODE_ASSINGMENT;
		node->value = NULL;

		ASTNode *expr = parse_expression(p);

		if(expr == NULL) {
			fprintf(stderr, "Expected expression after '='.\n");
 			exit(1);
		}

		node->assignment.target = id;
		node->assignment.value = expr;

		return node;

	}	
	return NULL;
}

// (((5 + 5) * (2)) + (2))
// ((5+5)*(2+2))
static ASTNode *innerExpression(Parser *p) {
	if(current(p)->type == TOKEN_LPARANTHES) {
		ASTNode *node = malloc(sizeof(ASTNode));

		advance(p);

		ASTNode *leftNode = innerExpression(p);

		advance(p);

		if(current(p)->type == TOKEN_INT || current(p)->type == TOKEN_IDENTIFIER) {
			leftNode->value = current(p)->value;
		} else {
			fprintf(stderr, "Error occured in expression, used something unidentified.\n");
		}

		expect(p, TOKEN_OPERATOR);

		if(strcmp(current(p)->value, "+") == 0) {
			node->type = NODE_PLUS;
		} else if(strcmp(current(p)->value, "-") == 0) {
			node->type = NODE_MINUS;
		} else if(strcmp(current(p)->value, "*") == 0) {
			node->type = NODE_MULTIPLY;
		} else if(strcmp(current(p)->value, "/") == 0) {
			node->type = NODE_DIVIDE;
		} 

		if(current(p)->type == TOKEN_RPARANTHES) {
			return node;
		}

		ASTNode *rightNode = innerExpression(p);

		advance(p);

		if(current(p)->type == TOKEN_INT || current(p)->type == TOKEN_IDENTIFIER) {
			rightNode->value = current(p)->value;
		} else {
			fprintf(stderr, "Error occured in expression, used something unidentified.\n");
		}

		expect(p, TOKEN_RPARANTHES);

		return node;


	}
	return NULL;
}

ASTNode *parse_expression(Parser *p) {
	ASTNode *node = malloc(sizeof(ASTNode));

	if(current(p)->type == TOKEN_LPARANTHES) {
		node->type = NODE_EXPRESSION;

		while(current(p)->type != TOKEN_EOL) {
			if(current(p)->type == TOKEN_LPARANTHES) {
				ASTNode *innerNode = malloc(sizeof(ASTNode));

				advance(p);
				innerNode->value = current(p)->value;
				expect(p, TOKEN_INT);

				if(current(p)->type == TOKEN_OPERATOR) {
					if(strcmp(current(p)->value, "+") == 0) {
						
					} else if(strcmp(current(p)->value, "-") == 0) {
						continue;
					} else if(strcmp(current(p)->value, "*") == 0) {
						continue;
					} else if(strcmp(current(p)->value, "/") == 0) {
						continue;
					} else {
						fprintf(stderr, "Unidentified operator used in the expression.\n");
					}
				}

			}
			if(current(p)->type == TOKEN_RPARANTHES) {
				break;
			}

			advance(p);
		}

	} else if(current(p)->type == TOKEN_STRING) {
		node->type = NODE_STRING;
		node->value = current(p)->value;

	} else if(current(p)->type == TOKEN_INT) {
		node->type = NODE_INT;
		node->value = current(p)->value;

	} else if(current(p)->type == TOKEN_IDENTIFIER) {
		node->type = NODE_IDENTIFIER;
		node->value = current(p)->value;

	}

	return node;
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
				fprintf(stderr, "Nothing to print.\n");
				exit(1);
			}

			node->print.expr = expr;

			return node;
		}
	}
	return NULL;
}

void clean_ASTs() {
	size_t pos = 0;
	while(program_node->program.statements[pos] != NULL) {

		pos++;
	}

	free(program_node->program.statements);
	free(program_node);
}

void display_ASTs() {
	size_t pos = 0;
	printf("Program\n");

	while(program_node->program.statements[pos] != NULL) {
		ASTNode *statement = program_node->program.statements[pos];

		if(statement->type == NODE_ASSINGMENT) {
			ASTNode *target = statement->assignment.target;
			ASTNode *value = statement->assignment.value;

			printf("target: %s, value: %s", target->value, NULL);
		}
		pos++;
	}

}