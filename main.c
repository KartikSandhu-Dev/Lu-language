#include <stdio.h>
#include <stdlib.h>
#include "lexer/lexer.h"
#include "parser/parser.h"

char *read_whole_file(char *name);

int main(int argc, char *argv[]) {
	(void)argc;
	char *buffer = read_whole_file(argv[1]);

	Token *tokens = tokenize(buffer);

		printf("----------TOKENS--------------\n");
	display_tokens(tokens);

	Parser p = {tokens, 0};
	ASTNode *tree = parse_program(&p);

	clean_tokens();

	printf("----------TREE--------------\n");
	print_ASTs(tree, 0, true);

	clean_ASTs(tree);

	return 0;
}

char *read_whole_file(char *name) {
	FILE *file;

	file = fopen(name, "r");

	if(!file) {
		fprintf(stderr, "FILE IS EMPTY!\n");
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	char *buffer = malloc(sizeof(char)*(size+1));
	fread(buffer, 1, size, file);

	buffer[size] = '\0';

	fclose(file);

	return buffer;
}