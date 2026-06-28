#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stdlib.h>
#include <stdbool.h>

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
	NODE_PROGRAM,
	NODE_BLOCK,

	NODE_INT,
	NODE_STRING,
	NODE_IDENTIFIER,

	NODE_PLUS,
	NODE_MULTIPLY,
	NODE_DIVIDE,
	NODE_MINUS,

	NODE_EQUALS,
	NODE_GREATTHAN,
	NODE_LESSTHAN,
	NODE_NOTEQUALS,

	NODE_ASSIGNMENT,
	NODE_PRINT,
	NODE_IF,
} NodeType;

typedef struct ASTNode {
	NodeType type;

	union {
		struct {
			ASTNode **statements;
			size_t capacity;
			size_t count;
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

		struct {
			ASTNode *condition;
			ASTNode *ifBody;
			ASTNode *elseBody;
		} ifelse;
	};

	union {
		int int_value;
		char *string_value;
		char *identifier_value;
	};

} ASTNode;

Token *current(Parser *p);
void advance(Parser *p);
void previous(Parser *p);

ASTNode *parse_program(Parser *p);
ASTNode *parse_statement(Parser *p);

ASTNode *parse_assignment(Parser *p);
ASTNode *parse_print(Parser *p);

ASTNode *parse_ifelse(Parser *p);
ASTNode *parse_ifstatements(Parser *p);

ASTNode *parse_expression(Parser *p);
ASTNode *parse_arithmetic(Parser *p);

void clean_ASTs(ASTNode *node);
void print_ASTs(ASTNode *node, int indent, bool isLast);

#endif