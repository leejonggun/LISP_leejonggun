#include <stdio.h>
#include "lisp.h"
#include <readline/readline.h>


int main(int argc, char** argv) {
	char *input = NULL;
	while((input = readline(">>> "))){
		int tmp = 0;
		while (tmp != (strlen(input))) {
			tmp = tokenize(input, tmp);
		}
	}
	return 0;
}


