#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

typedef enum TokenType {
	TOKEN_INT, //123
	TOKEN_STRING, //"hello", 'hello'
	TOKEN_ASSIGNMENT, // =
	TOKEN_LPARANTHES, // (
	TOKEN_RPARANTHES, // )
	TOKEN_OPERATOR, // * + - /
	TOKEN_LOGICALOP, // == > < !=
	TOKEN_IDENTIFIER,  //abc
	TOKEN_KEYWORD, // if else end print
	TOKEN_EOL, //; end of line
	TOKEN_SOC, //: start of block
	TOKEN_EOF, //NULL
} TokenType;

typedef struct Token {
	TokenType type;
	char *value;
} Token;

Token *tokenize(char *buffer);
void display_tokens(Token *tokens);
void clean_tokens();

#endif