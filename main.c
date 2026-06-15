#include <stdio.h>
#include <stdlib.h>
#include "header/lexer.h"

char *read_whole_file(char *name);

int main() {
	char *buffer = read_whole_file("prac.lu");

	display_tokens(tokenize(buffer));
	clean_tokens();

	return 0;
}

char *read_whole_file(char *name) {
	FILE *file;

	file = fopen(name, "r");

	if(!file) {
		fprintf(stderr, "FILE IS EMPTY\n");
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