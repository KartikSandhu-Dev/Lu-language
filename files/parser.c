#include "../header/parser.h"
#include "../header/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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
static void expect(Parser *p, TokenType type, char* errMessage) {
	if(current(p)->type != type) {
		fprintf(stderr, "%s\n", errMessage);
		exit(1);
	}
	advance(p);
}

// PARENT NODE
ASTNode *parse_program(Parser *p) {
	ASTNode *progNode = malloc(sizeof(ASTNode));
	progNode->type = NODE_PROGRAM;

	size_t *capacity = &progNode->program.capacity;
	size_t *count = &progNode->program.count;

	*capacity = 10;
	*count = 0;

	progNode->program.statements = malloc(sizeof(ASTNode*)*(*capacity));

	while(current(p)->type != TOKEN_EOF) {
		ASTNode *node = parse_statement(p);

		if(node != NULL) {
			progNode->program.statements[*count] = node;
			(*count)++;
		}

		if(*count >= *capacity) {
			*capacity*=2;
			progNode->program.statements = 
					realloc(progNode->program.statements, 
						sizeof(ASTNode*)*(*capacity));
		}
	}


	progNode->program.statements[*count] = NULL;

	return progNode;
}

ASTNode *parse_statement(Parser *p) {
	ASTNode *node = parse_assignment(p);
	if(node != NULL) {
		expect(p, TOKEN_EOL, "Incorrect syntax no EOL used.");
		return node;
	}
	
	node = parse_print(p);
	if(node != NULL) {
		expect(p, TOKEN_EOL, "Incorrect syntax no EOL used.");
		return node;
	}

	node = parse_ifelse(p);
	if(node != NULL) {
		return node;
	}

	if(current(p)->type == TOKEN_EOL) {
		advance(p);
		return NULL;
	}

	fprintf(stderr, "Unknown statement.\n");
	exit(1);
}

ASTNode *parse_assignment(Parser *p) {
	if(current(p)->type == TOKEN_IDENTIFIER) {
		ASTNode *id = malloc(sizeof(ASTNode));
		id->type = NODE_IDENTIFIER;
		id->value = current(p)->value;

		advance(p);
		expect(p, TOKEN_ASSIGNMENT, "Expected assignment.");

		ASTNode *node = malloc(sizeof(ASTNode));
		node->type = NODE_ASSIGNMENT;
		node->value = NULL;

		ASTNode *expr = parse_expression(p);

		node->assignment.target = id;
		node->assignment.value = expr;

		return node;

	}	
	return NULL;
}

// (((5 + 5) * 2) + 2)
// ((5+5)*(2+2))
ASTNode *parse_arithmetic(Parser *p) {
	if(current(p)->type == TOKEN_INT || current(p)->type == TOKEN_IDENTIFIER) {
		ASTNode *node = malloc(sizeof(ASTNode));

		if(current(p)->type == TOKEN_INT){
			node->type = NODE_INT;
		} else if(current(p)->type == TOKEN_IDENTIFIER) {
			node->type = NODE_IDENTIFIER;
		}

		node->value = current(p)->value;
		advance(p);
		return node;

	}

	expect(p, TOKEN_LPARANTHES, "Expected '(' in the expression.");

	ASTNode *left = parse_arithmetic(p);

	ASTNode *node = malloc(sizeof(ASTNode));

	switch(current(p)->value[0]) {
		case '+': node->type = NODE_PLUS; break;
		case '-': node->type = NODE_MINUS; break;
		case '*': node->type = NODE_MULTIPLY; break;
		case '/': node->type = NODE_DIVIDE; break;
		default:
			fprintf(stderr, "Unknown operator %s.\n", current(p)->value);
			exit(1);
	}

	expect(p, TOKEN_OPERATOR, "Expected an operator in the expression.");

	ASTNode *right = parse_arithmetic(p);

	expect(p, TOKEN_RPARANTHES, "Expected ')' in the expression.");

	node->expression.left = left;
	node->expression.right = right;

	return node;
}

ASTNode *parse_expression(Parser *p) {
	if(current(p)->type == TOKEN_STRING) {
	    ASTNode *node = malloc(sizeof(ASTNode));
	    node->type = NODE_STRING;
	    node->value = current(p)->value;
	    advance(p);
	    return node;
	}

	if(current(p)->type == TOKEN_INT ||
	    current(p)->type == TOKEN_IDENTIFIER ||
	    current(p)->type == TOKEN_LPARANTHES) {
	    return parse_arithmetic(p);
	}

	fprintf(stderr, "Expected expression.\n");
	exit(1);
}

ASTNode *parse_print(Parser *p) {
	if(current(p)->type == TOKEN_KEYWORD && (strcmp(current(p)->value, "print") == 0)) {
		advance(p);

	   	ASTNode *node = malloc(sizeof(ASTNode));
		node->type = NODE_PRINT;
		node->value = NULL;

		ASTNode *expr = parse_expression(p);

		node->print.expr = expr;

		return node;
		
	}
	return NULL;
}

ASTNode *parse_ifStatements(Parser *p) {
	ASTNode *progNode = malloc(sizeof(ASTNode));
	progNode->type = NODE_BLOCK;

	size_t *capacity = &progNode->program.capacity;
	size_t *count = &progNode->program.count;

	*capacity = 10;
	*count = 0;

	progNode->program.statements = malloc(sizeof(ASTNode*)*(*capacity));

	while(strcmp(current(p)->value, "end") != 0 && strcmp(current(p)->value, "else") != 0
		&& current(p)->type != TOKEN_EOF) {
		ASTNode *node = parse_statement(p);

		if(node != NULL) {
			progNode->program.statements[*count] = node;
			(*count)++;
		}

		if(*count >= *capacity) {
			*capacity*=2;
			progNode->program.statements =
					realloc(progNode->program.statements, 
						sizeof(ASTNode*)*(*capacity));
		}
	}

	if(current(p)->type == TOKEN_EOL) {
		fprintf(stderr, "Expected an 'end' after the if statemtn.\n");
		exit(1);
	}

	progNode->program.statements[*count] = NULL;

	return progNode;
}

ASTNode *parse_ifelse(Parser *p) {
	if(current(p)->type == TOKEN_KEYWORD && strcmp(current(p)->value, "if") == 0) {
		ASTNode *node = malloc(sizeof(ASTNode));
		node->type = NODE_IF;
		node->ifelse.ifBody = NULL;
		node->ifelse.elseBody = NULL;
		node->ifelse.condition = NULL;

		advance(p);

		ASTNode *leftExpr = parse_expression(p);
		ASTNode *logicNode = NULL;
		ASTNode *rightExpr = NULL;

		if(current(p)->type == TOKEN_LOGICALOP) {
			logicNode = malloc(sizeof(ASTNode));

			if(strcmp(current(p)->value, "==") == 0) {
				logicNode->type = NODE_EQUALSTO;
			} else if(strcmp(current(p)->value, ">") == 0) {
				logicNode->type = NODE_GREATERTHAN;
			} else if(strcmp(current(p)->value, "<") == 0) {
				logicNode->type = NODE_LESSERTHAN;
			} else if(strcmp(current(p)->value, "~=") == 0) {
				logicNode->type = NODE_NOTEQUALSTO;
			} else {
 				fprintf(stderr, "Unknown logical operator '%s'\n", current(p)->value);
  				exit(1);
			}

			advance(p);

			rightExpr = parse_expression(p);

			logicNode->expression.left = leftExpr;
			logicNode->expression.right = rightExpr;

			node->ifelse.condition = logicNode;

		} else {
			node->ifelse.condition = leftExpr;
		}

		expect(p, TOKEN_SOC, "No start of condition initalised.");

		node->ifelse.ifBody = parse_ifStatements(p);

		if(strcmp(current(p)->value, "else") == 0) {
			advance(p);
			expect(p, TOKEN_SOC, "No start of condition initalised.");
			node->ifelse.elseBody = parse_ifStatements(p);
		}

		if(strcmp(current(p)->value, "end") == 0) {
			advance(p);
			return node;
		}
	}

	return NULL;
}

void clean_ASTs(ASTNode *node) {
	if(!node) { return; }

	if(node->type == NODE_PROGRAM || node->type == NODE_BLOCK) {
		size_t pos = 0;
		while(node->program.statements[pos] != NULL) {
			clean_ASTs(node->program.statements[pos]);
			pos++;
		}
		free(node->program.statements);
		free(node);

	} else if(node->type == NODE_ASSIGNMENT) {
  		clean_ASTs(node->assignment.target);
 	  	clean_ASTs(node->assignment.value);
 	  	free(node);

	} else if(node->type == NODE_PRINT) {
		clean_ASTs(node->print.expr);
		free(node);

	} else if(node->type == NODE_IF) {
		clean_ASTs(node->ifelse.condition);
		clean_ASTs(node->ifelse.ifBody);
		clean_ASTs(node->ifelse.elseBody);
		free(node);

	} else if(node->type == NODE_PLUS || node->type == NODE_MINUS ||
		node->type == NODE_MULTIPLY || node->type == NODE_DIVIDE) {
  	  	clean_ASTs(node->expression.left);
   		clean_ASTs(node->expression.right);
   		free(node);
	} else if(node->type == NODE_EQUALSTO || node->type == NODE_NOTEQUALSTO ||
		node->type == NODE_GREATERTHAN || node->type == NODE_LESSERTHAN) {
  	  	clean_ASTs(node->expression.left);
   		clean_ASTs(node->expression.right);
   		free(node);

	} else if(node->type == NODE_INT) {
		free(node);

	} else if(node->type == NODE_STRING) {
		free(node);

	} else if(node->type == NODE_IDENTIFIER) {
		free(node);

	}
}

static bool last_level[8];
static void print_indent(int indent) {
    for(int i = 0; i < indent; i++) {
        if(last_level[i]) {
        	printf("    ");
        } else {
        	printf("|   ");
        }
    }
}

void print_ASTs(ASTNode *node, int indent, bool isLast) {
	if(!node) { return ;}

	last_level[indent] = isLast;
	print_indent(indent);

	if(node->type != NODE_PROGRAM) {
		if(isLast) { printf("└──"); }
		else { printf("├──"); }
	}

	if(node->type == NODE_PROGRAM) {
		printf("Program\n");
		size_t pos = 0;
		while(node->program.statements[pos] != NULL) {
			if(node->program.statements[pos+1] == NULL) {
				print_ASTs(node->program.statements[pos], indent, true);
				break;
			}
			print_ASTs(node->program.statements[pos], indent, false);
			pos++;
		}

	} else if(node->type == NODE_BLOCK) {
		printf("Block\n");
		size_t pos = 0;
		while(node->program.statements[pos] != NULL) {
			if(node->program.statements[pos+1] == NULL) {
				print_ASTs(node->program.statements[pos], indent+1, true);
				break;
			}
			print_ASTs(node->program.statements[pos], indent+1, false);
			pos++;
		}

	} else if(node->type == NODE_ASSIGNMENT) {
  	  printf("Assignment\n");

  	  print_ASTs(node->assignment.target, indent+1, false);
 	  print_ASTs(node->assignment.value, indent+1, true);

	} else if(node->type == NODE_PRINT) {
		printf("Print\n");

		print_ASTs(node->print.expr, indent+1, true);

	} else if(node->type == NODE_IF) {
		printf("If\n");

		print_ASTs(node->ifelse.condition, indent+1, false);
		if(node->ifelse.elseBody == NULL) {
			print_ASTs(node->ifelse.ifBody, indent+1, true);
		} else {
			print_ASTs(node->ifelse.ifBody, indent+1, false);
			print_ASTs(node->ifelse.elseBody, indent+1, true);
		}

	} else if(node->type == NODE_PLUS) {
   		printf("Plus\n");

  	  	print_ASTs(node->expression.left, indent+1, false);
   		print_ASTs(node->expression.right, indent+1, true);

	} else if(node->type == NODE_MINUS) {
   		printf("Minus\n");

  	  	print_ASTs(node->expression.left, indent+1, false);
   		print_ASTs(node->expression.right, indent+1, true);

	} else if(node->type == NODE_MULTIPLY) {
   		printf("Multiply\n");

  	  	print_ASTs(node->expression.left, indent+1, false);
   		print_ASTs(node->expression.right, indent+1, true);

	} else if(node->type == NODE_DIVIDE) {
   		printf("Divide\n");

  	  	print_ASTs(node->expression.left, indent+1, false);
   		print_ASTs(node->expression.right, indent+1, true);

	} else if(node->type == NODE_EQUALSTO) {
   		printf("EqualsTo\n");

  	  	print_ASTs(node->expression.left, indent+1, false);
   		print_ASTs(node->expression.right, indent+1, true);

	} else if(node->type == NODE_NOTEQUALSTO) {
   		printf("NotEqualsTo\n");

  	  	print_ASTs(node->expression.left, indent+1, false);
   		print_ASTs(node->expression.right, indent+1, true);

	} else if(node->type == NODE_GREATERTHAN) {
   		printf("GreaterThan\n");

  	  	print_ASTs(node->expression.left, indent+1, false);
   		print_ASTs(node->expression.right, indent+1, true);

	} else if(node->type == NODE_LESSERTHAN) {
   		printf("LesserThan\n");

  	  	print_ASTs(node->expression.left, indent+1, false);
   		print_ASTs(node->expression.right, indent+1, true);

	} else if(node->type == NODE_INT) {
		printf("%s\n", node->value);

	} else if(node->type == NODE_STRING) {
		printf("%s\n", node->value);

	} else if(node->type == NODE_IDENTIFIER) {
		printf("%s\n", node->value);

	}

}