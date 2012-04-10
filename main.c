#include <stdio.h>
#include "lisp.h"
#include <readline/readline.h>

int main(int argc, char** argv) {
	char *input = NULL;
	while(input = readline(">>> ")){
	tokenize(input);
	}
	return 0;
}
