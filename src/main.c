#include "lexer.h"
#include "parser.h"
#include "vm/vm.h"

char *read_whole_file(char *name);

int main(int argc, char *argv[]) {
	(void)argc;
	//------------lexer--------------
	Token *tokens = tokenize(argv[1]);

	//------------parser--------------
	Parser p = {tokens, 0};
	ASTNode *tree = parse_program(&p);

	//------------vm|execution--------------
	VM vm;
	vm_init(&vm, tree);
	vm_run(&vm);

	//------------cleanup--------------
	clean_tokens();
	clean_ASTs(tree);

	return 0;
}
