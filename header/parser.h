#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stdlib.h>

/* TREE STRUCTURE
Program
├── Assignment
│   ├── x
│   └── 10
│── Print
│    └── x
└── If
    ├── BinaryExpression(==)
    │   ├── Identifier(x)
    │   └── Int(10)
    ├── Then
    │   └── Print
    │       └── String("hello")
    └── Else
        └── Print
            └── String("bye")
*/

typedef struct ASTNode ASTNode;

typedef struct Parser {
	Token *tokens;
	size_t pos;
} Parser;

typedef enum NodeType {
	NODE_INT,
	NODE_STRING,
	NODE_IDENTIFIER,

	NODE_ASSINGMENT,
	NODE_PRINT,
} NodeType;

typedef struct ASTNode {
	NodeType type;

	union {
		struct {
			ASTNode **statements;
			size_t size;
		} program;

		struct {
			ASTNode *left;
			ASTNode *right;
		} expression;

		struct {
			ASTNode *target;
			ASTNode *value;
		} assignment;

		struct {
			ASTNode *expr;
		} print;
	};

	char *value;
} ASTNode;

Token *current(Parser *p);
void advance(Parser *p);
void previous(Parser *p);

void parse_program(Parser *p);
ASTNode *parse_statement(Parser *p);

ASTNode *parse_assignment(Parser *p);
ASTNode *parse_print(Parser *p);

ASTNode *parse_if(Parser *p);
ASTNode *parse_expression(Parser *p);

void clean_ASTs();

#endif