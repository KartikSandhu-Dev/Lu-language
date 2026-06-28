#include "vm/instruction.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "vm/bytecode.h"

static bool is_int(char *input, size_t len);
static bool is_str(char *input, size_t len);
static char **split_line(char *buffer);
static char *strip_quotes(char *input, size_t len);

void fill_instructions(VM *vm, char *filepath)
{
	FILE *f = fopen(filepath, "r");
	if (!f) {
	perror("fopen");
	exit(1);
	}

	char line[256];

	while (fgets(line, sizeof(line), f)) {
		// remove newline
		line[strcspn(line, "\n")] = '\0'; 

		if(line[0] == '\0') { continue; }

		Instruction inst = {0};
		Value val = {0};

		char **out = split_line(line);
		if(!out) {continue;}

		inst.opcode = opcode_from_str(out[0]);

		if (out[1] || needs_input(inst.opcode)) {
			size_t str_len = strlen(out[1]);
			
		    if (is_int(out[1], str_len)) {
		        val.type = INTEGER;
		        val.int_value = atoi(out[1]);
		    }
		    else if (is_str(out[1], str_len)) {
		        val.type = STRING;
		        val.string_value = strip_quotes(out[1], str_len);
		    } else {
		    	fprintf(stderr, "BAD SYNTAX\n");
		    	exit(1);
		    }

		}

		inst.value = val;

		vm->program[vm->code_size] = inst;
		vm->code_size++;

		// free split result
		for (int i = 0; out[i]; i++) {
		    free(out[i]);
		}
		free(out);
	}

}

static bool is_int(char *input, size_t len) {
	if(len == 0) { return false; }

	bool is_int = true;
	for(size_t i = 0; len > i; i++) {
		if(!(isdigit(input[i]))) {
			is_int = false;
			return is_int;
		}
	}
	return is_int;
}

static bool is_str(char *input, size_t len) {
	if(len == 0) { return false; }

	if ((input[0] == '\'' || input[0]=='"') && 
		(input[len-1]=='\'' || input[len-1]=='"')) {
		return true;
	}

	return false;
}
	
static char **split_line(char *buffer) {
	size_t pos = 0;

	char **output = malloc(sizeof(char*)*3);
	size_t outpos = 0;

	char stack[256];
	size_t stackpos = 0;

	char is_string = 0;

	while (1) {
		if(stackpos >= 256) {
			fprintf(stderr, "BUFFER OVERFLOW!\n");
			exit(1);
		}

		if(!isspace(buffer[pos]) || is_string == 1) {

			stack[stackpos] = buffer[pos];
			stackpos++;

			if(buffer[pos] == '"' || buffer[pos] == '\'') {
				is_string += 1;
			}
			if(is_string == 2) {
				is_string = 0;
			}

		} else if(isspace(buffer[pos]) && is_string == 0) {

			if(stackpos > 0) {
				output[outpos] = malloc(sizeof(char)*(stackpos+1));
				stack[stackpos] = '\0';

				memcpy(output[outpos], stack, stackpos+1);
				outpos++;
				stackpos = 0;
			}

		}

		if(buffer[pos] == '\0'){
			if(stackpos > 0) {
				output[outpos] = malloc(sizeof(char)*(stackpos+1));
				stack[stackpos] = '\0';

				memcpy(output[outpos], stack, stackpos+1);
				outpos++;
				stackpos = 0;
			}

			output[outpos] = NULL;
			break;
		}

		pos++;
	}

	return output;
}

static char *strip_quotes(char *input, size_t len) {
	if(len < 2) { return strdup(input); }

	char *out = malloc(len - 1);
	memcpy(out, input+1, len-2);
	out[len-2] = '\0';

	return out;
}

