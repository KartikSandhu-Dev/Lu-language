#include "../header/lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char **split_stream;
Token *token_stream;

static int is_keyword(char *buffer) {
	return strcmp(buffer, "if") == 0 ||
           strcmp(buffer, "else") == 0 ||
           strcmp(buffer, "print") == 0 ||
           strcmp(buffer, "end") == 0;
}

static int is_operator(char* buffer) {
	return strcmp(buffer, "+") == 0 ||
		   strcmp(buffer, "-") == 0 ||
		   strcmp(buffer, "*") == 0;
}

static int is_logicalop(char* buffer) {
	return strcmp(buffer, "==") == 0 ||
		   strcmp(buffer, ">") == 0 ||
		   strcmp(buffer, "<") == 0 ||
		   strcmp(buffer, "!=") == 0;
}

static bool is_int(char *input, size_t len) {
	if(len == 0) { return false; }

	bool is_int = true;
	for(int i = 0; len > i; i++) {
		if(!(isdigit(input[i]))) {
			is_int = false;
			return is_int;
		}
	}
	return is_int;
}

static bool is_string(char *input, size_t len) {
	if(len == 0) { return false; }

	if ((input[0] == '\'' || input[0]=='"') && 
		(input[len-1]=='\'' || input[len-1]=='"')) {
		return true;
	}

	return false;
}

static bool is_special_char(char op, char input) {
	if(op == '|') {
		return (input == '\0' || 
				input == ';' || input == ':' ||
				input == ')' || input == '(');

	} else if(op == '&') {
		return (input != '\0' && 
				input != ';' && input != ':' &&
				input != ')' && input != '(');
	}
	return false;
}
	
static char **split(char *buffer) {
	size_t pos = 0;
	size_t size = strlen(buffer);

	char **output = malloc(sizeof(char*)*(size + 1));
	size_t outpos = 0;

	char stack[256];
	size_t stackpos = 0;

	char is_string = 0;

	while (1) {
		if(stackpos >= 256) {
			fprintf(stderr, "BUFFER OVERFLOW!\n");
			exit(1);
		}

		if((!isspace(buffer[pos]) || is_string == 1) && is_special_char('&', buffer[pos])) {

			stack[stackpos] = buffer[pos];
			stackpos++;

			if(buffer[pos] == '"' || buffer[pos] == '\'') {
				is_string += 1;
			}
			if(is_string == 2) {
				is_string = 0;
			}

		} else if((isspace(buffer[pos]) || is_special_char('|', buffer[pos])) && is_string == 0) {

			if(stackpos > 0) {
				output[outpos] = malloc(sizeof(char)*(stackpos+1));
				stack[stackpos] = '\0';

				memcpy(output[outpos], stack, stackpos+1);
				outpos++;
				stackpos = 0;
			}

			if(buffer[pos] == ';') {
				output[outpos] = strdup(";");
				outpos++;
			} else if(buffer[pos] == ':') {
				output[outpos] = strdup(":");
				outpos++;
			} else if(buffer[pos] == ')') {
				output[outpos] = strdup(")");
				outpos++;
			} else if(buffer[pos] == '(') {
				output[outpos] = strdup("(");
				outpos++;
			}

		}

		if(buffer[pos] == '\0'){
			output[outpos] = NULL;
			break;
		}

		pos++;
	}

	split_stream = output;
	return output;
}

static char *strip_quotes(char *input, size_t len) {
	if(len < 2) { return strdup(input); }

	char *out = malloc(len - 1);
	memcpy(out, input+1, len-2);
	out[len-2] = '\0';

	return out;
}

Token *tokenize(char *buffer) {
	char **input = split(buffer);
	size_t pos = 0;
	size_t size = strlen(buffer);

	Token *output = malloc(sizeof(Token)*(size +1));
	size_t outpos = 0;

	while(1) {
		Token tok;

		if(input[pos] == NULL) {
			tok.type = TOKEN_EOF;
			tok.value = input[pos];
			output[outpos] = tok;
			break;
		}

		size_t len = strlen(input[pos]);
		tok.value = input[pos];
		
		if(is_int(input[pos], len)) {
			tok.type = TOKEN_INT;

		} else if(is_string(input[pos], len)) {
			tok.type = TOKEN_STRING;
			tok.value = strip_quotes(input[pos], len);

		} else if(input[pos][0] == ';') {
			tok.type = TOKEN_EOL;

		} else if(input[pos][0] == ':') {
			tok.type = TOKEN_SOC;

		} else if(input[pos][0] == '=') {
			tok.type = TOKEN_ASSIGNMENT;

		} else if(input[pos][0] == '(') {
			tok.type = TOKEN_LPARANTHES;

		} else if(input[pos][0] == ')') {
			tok.type = TOKEN_RPARANTHES;

		} else if(is_operator(input[pos])) {
			tok.type = TOKEN_OPERATOR;

		} else if(is_logicalop(input[pos])) {
			tok.type = TOKEN_LOGICALOP;

		} else if(is_keyword(input[pos])) {
			tok.type = TOKEN_KEYWORD;

		} else if(!(is_keyword(input[pos]))) {
			tok.type = TOKEN_IDENTIFIER;

		} else {
			fprintf(stderr, "Token not recognised, error occured.\n");
			exit(1);
		}

		output[outpos] = tok;
		outpos++;
		pos++;
	}

	token_stream = output;
	return output;
}

void clean_tokens() {
	size_t pos = 0;
	while(1) {
		if(split_stream[pos] == NULL) {
			break;
		}
		free(split_stream[pos]);
		split_stream[pos] = NULL;

		if(token_stream[pos].type == TOKEN_STRING) {
			free(token_stream[pos].value);
			token_stream[pos].value = NULL;
		}

		pos++;
	}

	free(token_stream);
	free(split_stream);

	token_stream = NULL;
	split_stream = NULL;
}

void display_tokens(Token *tokens) {
	size_t pos = 0;
	while(1) {
		Token tok = tokens[pos];
		if(tok.type == TOKEN_INT){
			printf("INTEGER: %s\n", tokens[pos].value);

		} else if(tok.type == TOKEN_EOL) {
			printf("EOL: %s\n", tokens[pos].value);

		} else if(tok.type == TOKEN_IDENTIFIER) {
			printf("IDENTIFIER: %s\n", tokens[pos].value);

		} else if(tok.type == TOKEN_SOC) {
			printf("START OF COND: %s\n", tokens[pos].value);

		} else if(tok.type == TOKEN_KEYWORD) {
			printf("KEYWORD: %s\n", tokens[pos].value);

		} else if(tok.type == TOKEN_STRING) {
			printf("STRING: %s\n", tokens[pos].value);

		} else if(tok.type == TOKEN_ASSIGNMENT) {
			printf("ASSINGMENT: %s\n", tokens[pos].value);
			
		} else if(tok.type == TOKEN_LPARANTHES) {
			printf("LPARANTHES: %s\n", tokens[pos].value);
			
		} else if(tok.type == TOKEN_RPARANTHES) {
			printf("RPARANTHES: %s\n", tokens[pos].value);

		} else if(tok.type == TOKEN_OPERATOR) {
			printf("OPERATORS: %s\n", tokens[pos].value);

		} else if(tok.type == TOKEN_LOGICALOP) {
			printf("LOGICALOP: %s\n", tokens[pos].value);

		} else if(tok.type == TOKEN_EOF) {
			printf("EOF\n");
			return;
		}

		pos++;
	}
}
